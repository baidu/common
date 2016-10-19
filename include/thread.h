// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  COMMON_THREAD_H_
#define  COMMON_THREAD_H_

#include <pthread.h>
#include <string.h>
#include <functional>

namespace baidu {
namespace common {

class Thread {
public:
    Thread() {
        memset(&tid_, sizeof(tid_), 0);
    }
    bool Start(std::function<void ()> thread_proc) {
        user_proc_ = thread_proc;
        int ret = pthread_create(&tid_, NULL, ProcWrapper, this);
        return (ret == 0);
    }
    typedef void* (Proc)(void*);
    bool Start(Proc proc, void* arg) {
        int ret = pthread_create(&tid_, NULL, proc, arg);
        return (ret == 0);
    }
    bool Join() {
        int ret = pthread_join(tid_, NULL);
        return (ret == 0);
    }
private:
    Thread(const Thread&);
    void operator=(const Thread&);
    static void* ProcWrapper(void* arg) {
        reinterpret_cast<Thread*>(arg)->user_proc_();
        return NULL;
    }
private:
    std::function<void ()> user_proc_;
    pthread_t tid_;
};

}
}

#endif  // COMMON_THREAD_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
