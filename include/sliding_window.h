// Copyright (c) 2014, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  BAIDU_COMMON_SLIDING_WINDOW_H_
#define  BAIDU_COMMON_SLIDING_WINDOW_H_

#include <stdint.h>
#include <vector>
#include <map>
#include <boost/function.hpp>

#include "mutex.h"

namespace baidu {
namespace common {

template <typename Item>
class SlidingWindow {
public:
    typedef boost::function<void (int32_t, Item)> SlidingCallback;
    SlidingWindow(int32_t size, SlidingCallback callback)
      : bitmap_(NULL), items_(NULL), item_count_(0),
        callback_(callback), size_(size),
        base_offset_(0), ready_(0), notifying_(false),
        to_be_shrink_(false), expect_size_(size) {
        bitmap_ = new char[size];
        memset(bitmap_, 0, size);
        items_ = new Item[size];
    }
    ~SlidingWindow() {
        delete[] bitmap_;
        delete[] items_;
    }
    int32_t Size() const {
        return item_count_;
    }
    int32_t GetBaseOffset() const {
        return base_offset_;
    }
    void GetFragments(std::vector<std::pair<int32_t, Item> >* fragments) {
        MutexLock lock(&mu_);
        for (int i = 0; i < size_; i++) {
            if (bitmap_[(ready_ + i) % size_]) {
                fragments->push_back(std::make_pair(base_offset_+i, items_[(ready_ + i) % size_]));
            }
        }
    }
    void Notify() {
        mu_.AssertHeld();
        notifying_ = true;
        while (bitmap_[ready_] == 1) {
            mu_.Unlock();
            callback_(base_offset_, items_[ready_]);
            mu_.Lock("SlidingWindow::Notify relock");
            bitmap_[ready_] = 0;
            ++ready_;
            ++base_offset_;
            --item_count_;
            if (ready_ >= size_) {
                ready_ = 0;
                if (to_be_shrink_) {
                    to_be_shrink_ = false;
                    delete[] bitmap_;
                    delete[] items_;
                    bitmap_ = new char[expect_size_];
                    memset(bitmap_, 0, expect_size_);
                    items_ = new Item[expect_size_];
                    size_ = expect_size_;
                }
            }
        }
        notifying_ = false;
    }
    int32_t UpBound() const {
        MutexLock lock(&mu_);
        return base_offset_ + size_ - 1;
    }
    /// Add a new item to slinding window.
    //  Returns:
    ///     0, Add to receiving buf;
    ///     1, Already received
    ///    -1, Not in receiving window
    /// Notes:
    ///     There is no thread pool, so SlidingCallback would be called by Add.
    ///     Pay attention to a deadlock.
    int Add(int32_t offset, Item item) {
        MutexLock lock(&mu_, "Slinding Add", 50000);
        int32_t pos = offset - base_offset_;
        if (pos >= size_) {
            return -1;
        } else if (pos < 0) {
            return 1;
        }
        pos = (pos + ready_) % size_;
        if (bitmap_[pos]) {
            return 1;
        }
        bitmap_[pos] = 1;
        items_[pos] = item;
        ++item_count_;
        if (!notifying_) Notify();
        return 0;
    }
    void Resize(int new_size) {
        MutexLock lock(&mu_);
        if (new_size == size_ || new_size < 0) {
            return;
        }
        if (new_size > size_) {
            EnlargeSize(new_size);
        } else {
            ShrinkSize(new_size);
        }
    }
private:
    int GetLastItemPos() {
        mu_.AssertHeld();
        for (int i = size_ - 1; i >= 0; i--) {
            if (bitmap_[i]) {
                return i;
            }
        }
        return -1;
    }
    int GetFirstItemPos() {
        mu_.AssertHeld();
        for (int i = 0; i < size_; i++) {
            if (bitmap_[i]) {
                return i;
            }
        }
        return size_ + 1;
    }
    void EnlargeSize(int new_size) {
        mu_.AssertHeld();
        char* new_bitmap = NULL;
        Item* new_items = NULL;
        int last_non_empty_pos = GetLastItemPos();
        int first_non_empty_pos = GetFirstItemPos();
        // maybe shrink is unfinished
        if (to_be_shrink_)  {
            to_be_shrink_ = false;
        }
        if (last_non_empty_pos == -1) {
            //current window is empty
            new_bitmap = new char[new_size];
            memset(new_bitmap, 0, new_size);
            new_items = new Item[new_size];
        } else {
            assert(last_non_empty_pos >= first_non_empty_pos);
            new_bitmap = new char[new_size];
            new_items = new Item[new_size];
            memset(new_bitmap, 0, new_size);
            //don't use memcpy for item, because item maybe not a POD type
            if (last_non_empty_pos >= ready_) {
                for (int i = ready_, j = 0; i <= last_non_empty_pos; i++, j++) {
                    new_items[j] = items_[i];
                }
                memcpy(new_bitmap, bitmap_ + ready_, (last_non_empty_pos - ready_ + 1) * sizeof(char));
            }
            //TODO: improve here
            if (first_non_empty_pos < ready_) {
                for (int i = first_non_empty_pos; i < ready_; i++) {
                    new_items[size_ - ready_ + i] = items_[i];
                }
                memcpy(new_bitmap + size_ - ready_ + first_non_empty_pos,
                        bitmap_ + first_non_empty_pos,
                        (ready_ - first_non_empty_pos) * sizeof(char));
            }
        }
        delete[] bitmap_;
        delete[] items_;
        bitmap_ = new_bitmap;
        items_ = new_items;
        ready_ = 0;
        size_ = new_size;
    }
    void ShrinkSize(int new_size) {
        //TODO: deal with multiple shrink
        mu_.AssertHeld();
        int last_non_empty_pos = GetLastItemPos();
        int first_non_empty_pos = GetFirstItemPos();
        char* new_bitmap = NULL;
        Item* new_items = NULL;
        if (last_non_empty_pos == -1) {
            // current window is empty
            new_bitmap = new char[new_size];
            memset(new_bitmap, 0, new_size);
            new_items = new Item[new_size];
            memset(new_items, 0, new_size);
            size_ = new_size;
        } else {
            assert(last_non_empty_pos >= first_non_empty_pos);
            int tmp_size = last_non_empty_pos - first_non_empty_pos + 1;
            new_bitmap = new char[tmp_size];
            //no need to memset
            memcpy(new_bitmap, bitmap_ + first_non_empty_pos, (tmp_size) * sizeof(char));
            new_items = new Item[tmp_size];
            for (int i = 0; i < tmp_size; i++) {
                new_items[i] = items_[i + first_non_empty_pos];
            }
            size_ = tmp_size;
            expect_size_ = new_size;
            to_be_shrink_ = true;
        }
        delete[] bitmap_;
        delete[] items_;
        bitmap_ = new_bitmap;
        items_ = new_items;
        ready_ = 0;
    }
private:
    char* bitmap_;
    Item* items_;
    int32_t item_count_;
    SlidingCallback callback_;
    int32_t size_;
    int32_t base_offset_;
    int32_t ready_;
    bool notifying_;
    bool to_be_shrink_;
    int32_t expect_size_;
    mutable Mutex mu_;
};

} // namespace common
} // namespace baidu

#endif  // BAIDU_COMMON_SLIDING_WINDOW_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
