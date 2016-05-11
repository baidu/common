// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <gtest/gtest.h>
#include <boost/bind.hpp>

#include <mutex.h>
#include <thread.h>

namespace baidu {
namespace common {

class ThreadTest : public ::testing::Test {
public:
    ThreadTest() : task_done_(&task_mu_) {}
    void Task() {
        MutexLock l(&task_mu_);
        task_done_.Signal();
    }

protected:
    CondVar task_done_;
    mutable Mutex task_mu_;
};

TEST_F(ThreadTest, Start) {
    Thread t;
    MutexLock l(&task_mu_);
    t.Start(boost::bind(&ThreadTest::Task, this));
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
