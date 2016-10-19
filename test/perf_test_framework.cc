// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <counter.h>
#include <mutex.h>
#include <thread.h>
#include <unistd.h>

#include <sys/time.h>
#include <stdio.h>
#include <queue>
#include <list>

namespace baidu {
namespace common {

Counter counter;

void Atomic() {
    while (1) {
        counter.Inc();
    }
}

void TestGettime() {
    while (1) {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        counter.Inc();
    }
}

void TestMutex() {
    Mutex mu;
    while (1) {
        {
            mu.Lock();
            mu.Unlock();
        }
        counter.Inc();
    }
}

void TestMutexLock() {
    Mutex mu;
    while (1) {
        {
            MutexLock lock(&mu);
        }
        counter.Inc();
    }
}

void TestRawMutex() {
    pthread_mutex_t m;
    pthread_mutex_init(&m, NULL);
    while (1) {
        {
            pthread_mutex_lock(&m);
            pthread_mutex_unlock(&m);
        }
        counter.Inc();
    }
    pthread_mutex_destroy(&m);
}

int Run(int argc, char* argv[]) {
    if (argc < 3) {
        printf("%s function thread_num\n", argv[0]);
        return 0;
    }
    void (*task)() = Atomic;
    if (strcmp(argv[1], "mutex") == 0) {
        printf("Perf test mutex\n");
        task = TestMutex;
    } else if (strcmp(argv[1], "rawmutex") == 0) {
        printf("Perf test raw mutex\n");
        task = TestRawMutex;
    } else if (strcmp(argv[1], "mutexlock") == 0) {
        printf("Perf test MutexLock\n");
        task = TestMutexLock;
    } else if (strcmp(argv[1], "gettime") == 0) {
        printf("Perf test gettimeofday\n");
        task = TestGettime;
    }

    int thread_num = 10;
    thread_num = atoi(argv[2]);

    printf("Thread num %d\n", thread_num);

    Thread* t = new Thread[thread_num];
    for (int i = 0; i < thread_num; i++) t[i].Start(task);
    while (1) {
        sleep(1);
        fprintf(stderr, "%ld\n", counter.Clear());
    }
    return 0;
}
}
}

int main(int argc, char* argv[]) {
    return baidu::common::Run(argc, argv);
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
