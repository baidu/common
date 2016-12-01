// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef  COMMON_RWLOCK_H_
#define  COMMON_RWLOCK_H_

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#include "thread.h"
#include "mutex.h"
#include "timer.h"

namespace baidu {
namespace common {

class RWLock {
public:
    RWLock() : msg_(NULL), msg_threshold_(0), lock_time_(0) {
        pthread_rwlockattr_t attr;
        pthread_rwlockattr_init(&attr);
        pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
        pthread_rwlock_init(&rw_lock_, &attr);
    }
    void ReadLock(const char* msg = NULL, int64_t msg_threshold = 5000) {
        #ifdef RWLOCK_DEBUG
        int64_t s = (msg) ? timer::get_micros() : 0;
        #endif
        pthread_rwlock_rdlock(&rw_lock_);
        AfterLock(msg, msg_threshold);
        #ifdef RWLOCK_DEBUG
        if (msg && lock_time_ - s > msg_threshold) {
            char buf[32];
            common::timer::now_time_str(buf, sizeof(buf));
            printf("%s [rwlock] %s wait lock %.3f ms\n",
                    buf, msg, (lock_time_ - s) / 1000.0);
        }
        #endif
    }
    void WriteLock(const char* msg = NULL, int64_t msg_threshold = 5000) {
        #ifdef RWLOCK_DEBUG
        int64_t s = (msg) ? timer::get_micros() : 0;
        #endif
        pthread_rwlock_wrlock(&rw_lock_);
        AfterLock(msg, msg_threshold);
        #ifdef RWLOCK_DEBUG
        if (msg && lock_time_ - s > msg_threshold) {
            char buf[32];
            common::timer::now_time_str(buf, sizeof(buf));
            printf("%s [rwlock] %s wait lock %.3f ms\n",
                    buf, msg, (lock_time_ - s) / 1000.0);
        }
        #endif
    }
    void Unlock() {
        BeforeUnlock();
        pthread_rwlock_unlock(&rw_lock_);
    }
private:
    void AfterLock(const char* msg, int64_t msg_threshold) {
        #ifdef RWLOCK_DEBUG
        msg_ = msg;
        msg_threshold_ = msg_threshold;
        if (msg_) {
            lock_time_ = timer::get_micros();
        }
        #endif
        (void)msg;
        (void)msg_threshold;
    }
    void BeforeUnlock() {
        #ifdef MUTEX_DEBUG
        if (msg_ && timer::get_micros() - lock_time_ > msg_threshold_) {
            char buf[32];
            common::timer::now_time_str(buf, sizeof(buf));
            printf("%s [rwlock] %s locked %.3f ms\n", 
                   buf, msg_, (timer::get_micros() - lock_time_) / 1000.0);
        }
        msg_ = NULL;
        #endif
    }
private:
    RWLock(const RWLock&);
    void operator=(const RWLock&);
    pthread_rwlock_t rw_lock_;
    const char* msg_;
    int64_t msg_threshold_;
    int64_t lock_time_;
};

} // namespace common
} // namespace baidu

#endif  // COMMON_RWLOCK_H_
