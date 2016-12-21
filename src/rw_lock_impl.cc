// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include "rw_lock_impl.h"

#include <syscall.h>
#include <linux/futex.h>
#include <limits.h>
#include <unistd.h>
#include <assert.h>

#include "mutex.h"

static inline int sys_futex(void *futex, int op, int val,
                            const struct timespec *timeout) {
    return syscall(__NR_futex, op, val, timeout);
}

namespace baidu {
namespace common {

RWLockImpl::RWLockImpl() : readers_size_(0), readers_wakeup_(0),
                       writer_wakeup_(0), readers_queue_size_(0),
                       writers_queue_size_(0), writer_tid_(0) {
}

int RWLockImpl::ReadLock() {
    mu_.Lock();
    while (true) {
        if (writer_tid_ == 0 && writers_queue_size_ == 0) {
            // no writer is writing or waiting, get the lock
            readers_size_++;
            mu_.Unlock();
            return 0;
        }
        readers_queue_size_++;
        int wait_val = readers_wakeup_;
        mu_.Unlock();
        sys_futex(&readers_wakeup_, FUTEX_WAIT, wait_val, NULL);
        mu_.Lock();
        readers_queue_size_--;
    }
}

int RWLockImpl::WriteLock() {
    mu_.Lock();
    while (true) {
        if (writer_tid_ == 0 && readers_size_ == 0) {
            //luckly we're the first one
            //TODO use tid here
            //TODO cache tid here
            writer_tid_ = 1;
            mu_.Unlock();
            return 0;
        }
        //TODO check deadlock
        writers_queue_size_++;
        int wait_val = writer_wakeup_;
        mu_.Unlock();
        sys_futex(&writer_wakeup_, FUTEX_WAIT, wait_val, NULL);
        mu_.Lock();
        writers_queue_size_--;
    }
}

int RWLockImpl::Unlock() {
    // don't use MutexLock, because we want to benefit from
    // memory barrier of pthread_mutex_lock/unlock
    mu_.Lock();
    assert(writer_tid_ != 0 || readers_size_ > 0);
    if (writer_tid_ != 0) {
        // we allow unlock in another thread, so don't check tid here
        writer_tid_ = 0;
    } else {
        readers_size_--;
    }

    if (readers_size_ == 0) {
        if (writers_queue_size_) {
            writer_wakeup_++;
            mu_.Unlock();
            sys_futex(&writer_wakeup_, FUTEX_WAKE, 1, NULL);
            return 0;
        } else if (readers_queue_size_) {
            readers_wakeup_++;
            mu_.Unlock();
            sys_futex(&readers_wakeup_, FUTEX_WAKE, INT_MAX, NULL);
            return 0;
        }
    }
    mu_.Unlock();
    return 0;
}

} // namespace common
} // namespace baidu
