// Copyright (c) 2015, Baidu.com, Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#include <logging.h>

#include <string>

int main(int argc, char* argv[]) {
    int test_time = 88;
    const char* char_pointer = "char*";
    std::string string = "std;";
    LOGS(baidu::INFO) << 88 << " " << char_pointer << " " << string;
    LOGS(baidu::INFO) << 88 << " " << char_pointer << " " << string;

    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
