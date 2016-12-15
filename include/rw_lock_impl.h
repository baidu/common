// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef  BAIDU_COMMON_RWLOCK_IMPL_H_
#define  BAIDU_COMMON_RWLOCK_IMPL_H_

#include "mutex.h"

namespace baidu {
namespace common {

class RWLockImpl {
public:
    RWLockImpl();
    int ReadLock();
    int WriteLock(); int Unlock();
private:
    unsigned int readers_size_;
    unsigned int readers_wakeup_;
    unsigned int writer_wakeup_;
    unsigned int readers_queue_size_;
    unsigned int writers_queue_size_;
    unsigned int writer_tid_;
    //TODO maybe pthread_mutex_lock is too expensive
    Mutex mu_;
};

} // namespace common
} // namespace baidu

#endif
