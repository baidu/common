// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <gtest/gtest.h>
#include <functional>

#include <thread_pool.h>

namespace baidu {
namespace common {

class ThreadPoolTest : public ::testing::Test {
public:
    ThreadPoolTest() : task_done_(&task_mu_) {}
    void Task() {
        MutexLock l(&task_mu_);
        task_done_.Signal();
    }

protected:
    CondVar task_done_;
    mutable Mutex task_mu_;
};

TEST_F(ThreadPoolTest, AddTask) {
    ThreadPool tp;
    MutexLock l(&task_mu_);
    tp.AddTask(std::bind(&ThreadPoolTest::Task, this));
    bool ret = task_done_.TimeWait(1000);
    ASSERT_TRUE(ret);
}

}
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
