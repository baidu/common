// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include "rw_lock_impl.h"

#include <syscall.h>
#include <linux/futex.h>
#include <limits.h>

#include "mutex.h"

#define futex_wait(futex, val) \
  ({									      \
    int __status;							      \
    register __typeof (val) _val asm ("edx") = (val);			      \
    __asm __volatile ("xorq %%r10, %%r10\n\t"				      \
		      "syscall"						      \
		      : "=a" (__status)					      \
		      : "0" (SYS_futex), "D" (futex), "S" (FUTEX_WAIT),	      \
			"d" (_val)					      \
		      : "memory", "cc", "r10", "r11", "cx");		      \
    __status;								      \
  })

#define futex_wake(futex, nr) \
  do {									      \
    int __ignore;							      \
    register __typeof (nr) _nr asm ("edx") = (nr);			      \
    __asm __volatile ("syscall"						      \
		      : "=a" (__ignore)					      \
		      : "0" (SYS_futex), "D" (futex), "S" (FUTEX_WAKE),	      \
			"d" (_nr)					      \
		      : "memory", "cc", "r10", "r11", "cx");		      \
  } while (0)

namespace baidu {
namespace common {

int RWLockImpl::ReadLock() {
    mu_.Lock();
    while (true) {
        if (writer_tid_ == 0) {
            // no writer now, get the lock
            readers_size_++;
            mu_.Unlock();
            return 0;
        }
        readers_queue_size_++;
        int wait_val = readers_wakeup_;
        mu_.Unlock();
        futex_wait(&readers_wakeup_, &wait_val);
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
        futex_wait(&writer_wakeup_, &wait_val);
        mu_.Lock();
        writers_queue_size_--;
    }
}

int RWLockImpl::Unlock() {
    // don't use MutexLock, because we want to benefit from
    // memory barrier of pthread_mutex_lock/unlock
    mu_.Lock();
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
            futex_wake(&writer_wakeup_, 1);
            return 0;
        } else if (readers_queue_size_) {
            readers_wakeup_++;
            mu_.Unlock();
            futex_wake(&readers_wakeup_, INT_MAX);
            return 0;
        }
    }
    mu_.Unlock();
    return 0;
}

} // namespace common
} // namespace baidu
