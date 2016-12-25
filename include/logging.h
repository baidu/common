// Copyright (c) 2014, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  BAIDU_COMMON_LOGGING_H_
#define  BAIDU_COMMON_LOGGING_H_

#include <sstream>

namespace baidu {
namespace common {

enum LogLevel {
    DEBUG = 2,
    INFO = 4,
    WARNING = 8,
    ERROR = 16,
    FATAL = 32,
};

void SetLogLevel(int level);
bool SetLogFile(const char* path, bool append = false);
bool SetWarningFile(const char* path, bool append = false);
bool SetLogSize(int size); // in MB
bool SetLogCount(int count);
bool SetLogSizeLimit(int size); // in MB

void Log(int level, const char* fmt, ...);

class LogStream {
public:
    LogStream(int level);
    template<class T>
    LogStream& operator<<(const T& t) {
        oss_ << t;
        return *this;
    }
    ~LogStream();
private:
    int level_;
    std::ostringstream oss_;
};

} // namespace common

using common::DEBUG;
using common::INFO;
using common::WARNING;
using common::ERROR;
using common::FATAL;

} // namespace baidu

#define LOG(level, fmt, args...) ::baidu::common::Log(level, "[%s:%d] " fmt, __FILE__, __LINE__, ##args)
#define LOGS(level) ::baidu::common::LogStream(level)

#endif  // BAIDU_COMMON_LOGGING_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
