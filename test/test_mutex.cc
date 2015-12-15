// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <mutex.h>
#include <thread.h>

#include <stdio.h>

baidu::common::Mutex x;
void LockFunc() {
    x.Lock();
}

void UnlockFunc() {
    x.Unlock();
}

int main() {
    baidu::common::Thread t1,t2;
    t1.Start(LockFunc);
    t1.Join();
    t2.Start(UnlockFunc);
    t2.Join();
    printf("Done\n");
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
