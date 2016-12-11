// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include <gtest/gtest.h>

#include <stdio.h>
#include <unistd.h>
#include <syscall.h>

#include <rw_lock.h>
#include <thread_pool.h>

baidu::common::RWLock rw_lock;

void UnlockFunc() {
    int32_t thread_id = syscall(__NR_gettid);
    printf("thread %d unlock\n", thread_id);
    rw_lock.Unlock();
}

void ReadLockFunc(bool unlock) {
    rw_lock.ReadLock();
    int32_t thread_id = syscall(__NR_gettid);
    printf("thread %d get read lock\n", thread_id);
    sleep(1);
    if (unlock) {
        UnlockFunc();
    }
}

void WriteLockFunc(bool unlock) {
    rw_lock.WriteLock();
    int32_t thread_id = syscall(__NR_gettid);
    printf("thread %d get write lock\n", thread_id);
    sleep(1);
    if (unlock) {
        UnlockFunc();
    }
}

namespace baidu {
namespace common {
    TEST(RWLockTest, BasicTest) {
        baidu::common::ThreadPool tp;
        tp.AddTask(std::bind(ReadLockFunc, true));
        tp.AddTask(std::bind(WriteLockFunc, true));
        tp.AddTask(std::bind(ReadLockFunc, true));
        tp.AddTask(std::bind(WriteLockFunc, true));
        tp.Stop(true);
    }
    TEST(RWLockTest, RandomTest) {
        srand(time(NULL));
        baidu::common::ThreadPool tp;
        for (int i = 0; i < 20; i++) {
            int tmp = rand() % 2;
            if (tmp) {
                tp.AddTask(std::bind(ReadLockFunc, true));
            } else {
                tp.AddTask(std::bind(WriteLockFunc, true));
            }
        }
        tp.Stop(true);
    }
    TEST(RWLockTest, UnlockInAnotherThreadTest) {
        baidu::common::ThreadPool tp;
        tp.AddTask(std::bind(ReadLockFunc, false));
        // wait for task to be executed
        sleep(2);
        tp.AddTask(std::bind(UnlockFunc));
        sleep(2);
        tp.AddTask(std::bind(WriteLockFunc, false));
        sleep(2);
        tp.AddTask(std::bind(UnlockFunc));
        tp.Stop(true);
    }
} // namespace common
} // namespace baidu

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
