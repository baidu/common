// Copyright (c) 2014, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  COMMON_TIMER_H_
#define  COMMON_TIMER_H_

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string>

namespace baidu {
namespace common {

namespace timer {

enum Precision {
    kDay,
    kMin,
    kUsec,
};

static inline long get_micros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<long>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

static inline int32_t now_time() {
    return static_cast<int32_t>(get_micros() / 1000000);
}

static inline int32_t now_time_str(char* buf, int32_t len, Precision p = kUsec) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    const time_t seconds = tv.tv_sec;
    struct tm t;
    localtime_r(&seconds, &t);
    int32_t ret = 0;
    if (p == kDay) {
        ret = snprintf(buf, len, "%02d/%02d",
                t.tm_mon + 1,
                t.tm_mday);
    } else if (p == kMin) {
        ret = snprintf(buf, len, "%02d/%02d %02d:%02d",
                t.tm_mon + 1,
                t.tm_mday,
                t.tm_hour,
                t.tm_min);
    } else {
        ret = snprintf(buf, len, "%02d/%02d %02d:%02d:%02d.%06d",
            t.tm_mon + 1,
            t.tm_mday,
            t.tm_hour,
            t.tm_min,
            t.tm_sec,
            static_cast<int>(tv.tv_usec));
    }
    return ret;
}

class AutoTimer {
public:
    AutoTimer(double timeout_ms = -1, const char* msg1 = NULL, const char* msg2 = NULL)
      : timeout_(timeout_ms),
        msg1_(msg1),
        msg2_(msg2) {
        start_ = get_micros();
    }
    int64_t TimeUsed() const {
        return get_micros() - start_;
    }
    ~AutoTimer() {
        if (timeout_ == -1) return;
        long end = get_micros();
        if (end - start_ > timeout_ * 1000) {
            double t = (end - start_) / 1000.0;
            if (!msg2_) {
                fprintf(stderr, "[AutoTimer] %s use %.3f ms\n",
                    msg1_, t);
            } else {
                fprintf(stderr, "[AutoTimer] %s %s use %.3f ms\n",
                    msg1_, msg2_, t);
            }
        }
    }
private:
    long start_;
    double timeout_;
    const char* msg1_;
    const char* msg2_;
};

class TimeChecker {
public:
    TimeChecker() {
        start_ = get_micros();
    }
    void Check(int64_t timeout, const std::string& msg) {
        int64_t now = get_micros();
        int64_t interval = now - start_;
        if (timeout == -1 || interval > timeout) {
            char buf[30];
            now_time_str(buf, 30);
            fprintf(stderr, "[TimeChecker] %s %s use %ld us\n", buf, msg.c_str(), interval);
        }
        start_ = get_micros();
    }
    void Reset() {
        start_ = get_micros();
    }
private:
    int64_t start_;
};

}
}
} // namespace baidu

#endif  // COMMON_TIMER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
