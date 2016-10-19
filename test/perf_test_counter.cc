// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <thread.h>
#include <counter.h>
#include <unistd.h>

const int kThreadNum = 8;

namespace baidu {
namespace common {



void* AddProc(void* arg) {
    Counter* counter = reinterpret_cast<Counter*>(arg);
    while (1) {
        counter->Inc();
    }
}

void RunPerfTest() {
    Counter counter;
    Thread* threads = new Thread[kThreadNum];
    for (int i = 0; i < kThreadNum; i++) {
        threads[i].Start(AddProc, &counter);
    }

    long s_time = timer::get_micros();
    while (1) {
        usleep(1000000);
        long n_time = timer::get_micros();
        long count = counter.Clear();
        printf("%ld\n", count * 1000000 / (n_time - s_time));
        s_time = n_time;
    }
}
}
}

int main(int argc, char* argv[]) {
    baidu::common::RunPerfTest();
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
