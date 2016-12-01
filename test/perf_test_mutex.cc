// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <counter.h>
#include <mutex.h>
#include <thread.h>
#include <unistd.h>

#include <stdio.h>

namespace baidu {
namespace common {

Mutex mu;
CondVar p(&mu);
CondVar c(&mu);
Counter items;
Counter counter;

void Consumer() {
    MutexLock lock(&mu);
    while(1) {
        while(items.Get() == 0) {
            p.Wait();
        }
        //printf("Consume\n");
        items.Clear();
        counter.Inc();
        c.Signal();
    }
}

void Producer() {
    MutexLock lock(&mu);
    while (1) {
        while (items.Get() > 0) {
            c.Wait();
        }
        //printf("Produce\n");
        items.Inc();
        p.Signal();
    }
}

int Run() {
    baidu::common::Thread t1,t2;
    t1.Start(Consumer);
    t2.Start(Producer);
    while (1) {
        sleep(1);
        fprintf(stderr, "%ld\n", counter.Clear());
    }
    return 0;
}
}
}

int main() {
    return baidu::common::Run();
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
