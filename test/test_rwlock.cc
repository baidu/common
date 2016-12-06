// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#include <stdio.h>
#include <unistd.h>

#include <rw_lock.h>
#include <thread_pool.h>

baidu::common::RWLock rw_lock;

void UnlockFunc() {
    rw_lock.Unlock();
}

void ReadLockFunc() {
    rw_lock.ReadLock();
    sleep(10);
    UnlockFunc();
}

void WriteLockFunc() {
    rw_lock.WriteLock();
    UnlockFunc();
}

int main() {
    baidu::common::ThreadPool tp;
    tp.AddTask(std::bind(ReadLockFunc));
    tp.AddTask(std::bind(WriteLockFunc));
    tp.AddTask(std::bind(ReadLockFunc));
    tp.AddTask(std::bind(WriteLockFunc));
    tp.Stop(true);
    printf("test passed\n");
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
