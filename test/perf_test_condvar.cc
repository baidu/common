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
#include <queue>

int* ring __attribute__((aligned(64)));
int r_len __attribute__((aligned(64))) = 102400;
volatile long r_s __attribute__((aligned(64))) = 0;
volatile long r_e __attribute__((aligned(64))) = 0;


namespace baidu {
namespace common {

Mutex mu __attribute__((aligned(64)));
int g_xx1 __attribute__((aligned(64)));
CondVar p(&mu);
int g_xx2 __attribute__((aligned(64)));
CondVar c(&mu);
Counter items __attribute__((aligned(64)));

std::queue<int> queue __attribute__((aligned(64)));

Counter counter __attribute__((aligned(64)));

void Consumer() {
    //sleep(5);
    while(1) {
        /*while(items.Get() == 0) {
            MutexLock lock(&mu);
            p.Wait();
        }*/
        //items.Dec();
        //int v;
        //queue.pop();
        //ring[atomic_add64(&r_s, 1)%r_len] = -1;
        counter.Inc();
        c.Signal();
    }
}

void Producer() {
    while (1) {
        while (items.Get() > 100000) {
            MutexLock lock(&mu);
            c.Wait();
        }
        items.Inc();
        //queue.push(1);
        //ring[atomic_add64(&r_e, 1)%r_len] = 1;
        //p.Signal();
    }
}

int Run(int tnum) {
    Thread* t1 = new Thread[tnum];
    Thread* t2 = new Thread[tnum];
    printf("Thread num: %d\n", tnum);
    for (int i = 0; i < tnum; i++) t1[i].Start(Consumer);
    for (int i = 0; i < tnum; i++) t2[i].Start(Producer);
    while (1) {
        sleep(1);
        fprintf(stderr, "%ld %ld\n", counter.Clear(), items.Get());
    }
    return 0;
}
}
}

int main(int argc, char* argv[]) {
    int tnum = 1;
    if (argc > 1) {
        tnum = atoi(argv[1]);
    }
    ring = new int[r_len];
    return baidu::common::Run(tnum);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
