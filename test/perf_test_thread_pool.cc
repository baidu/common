// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <thread.h>
#include <thread_pool.h>


const int kThreadNum = 8;
const int kMaxPending = 10000;

namespace baidu {
namespace common {

void Task() {
}

void* AddTask(void* arg) {
    ThreadPool* tp = reinterpret_cast<ThreadPool*>(arg);
    while (1) {
        while (tp->PendingNum() > kMaxPending) {
        }
        tp->AddTask(Task);
    }
    return NULL;
}

void RunPerfTest() {
    ThreadPool tp;
    Thread* threads = new Thread[kThreadNum];
    for (int i = 0; i < kThreadNum; i++) {
        threads[i].Start(AddTask, &tp);
    }

    long s_time = timer::get_micros();
    while (1) {
        usleep(1000000);
        long n_time = timer::get_micros();
        std::string plog = tp.ProfilingLog();
        long pending = tp.PendingNum();
        printf("%ld %s\n", pending, plog.c_str());
    }
}
}
}

int main(int argc, char* argv[]) {
    baidu::common::RunPerfTest();
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
