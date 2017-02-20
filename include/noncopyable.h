// Copyright (c) 2017, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef BAIDU_COMMON_NONCOPYABLE_H_
#define BAIDU_COMMON_NONCOPYABLE_H_

namespace baidu {
namespace common {

class noncopyable {
public:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable(const noncopyable&) {}
  void operator=(const noncopyable&) {}
};
 
} // namespace common
} // namespace baidu

#endif  // BAIDU_COMMON_NONCOPYABLE_H_
