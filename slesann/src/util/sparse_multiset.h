/*
 * sparse_multiset.h
 *
 * Copyright (C) 2011 Leo Osvald <leo.osvald@gmail.com>
 *
 * This file is part of SLESANN.
 *
 * SLESANN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SLESANN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SLESANN. If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * sparse_multiset.h
 *
 *  Created on: May 28, 2011
 *      Author: losvald
 */

#ifndef SPARSE_MULTISET_H_
#define SPARSE_MULTISET_H_

#include <cassert>
#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <iostream>

namespace {

}  // namespace

template<typename T, int N, bool WrapAround = false>
class SparseMultiset {
public:
  SparseMultiset() {
    Clear();
  }

  void Add(const T& t) {
    ++arr_[ToIndex(t)];
    ++size_;
  }

  void AddMultiple(const T& t, std::size_t count) {
    for (std::size_t i = 0; i < count; ++i)
      Add(t);
  }

  bool Remove(const T& e) {
    std::size_t e_ind = ToIndex(e);
    return arr_[e_ind] && --arr_[e_ind] >= 0 && --size_ >= 0;
  }

  std::size_t Count(const T& t) {
    return arr_[ToIndex(t)];
  }

  void Clear() {
    std::fill(arr_, arr_ + N, T());
    size_ = 0;
  }

  T FindMin() const {
    if (size_ == 0)
      return FromIndex(kSize);

    std::size_t min_ind = 0, min = arr_[0];
    if (min < kSize) {
      for (std::size_t i = 1; i < kSize; ++i)
        if (arr_[i] < arr_[min_ind])
          min = arr_[min_ind = i];
    }
    return FromIndex(min_ind);
  }

  T FindMax() const {
    if (size_ == 0)
      return FromIndex(kSize);

    std::size_t max_ind = 0, max = arr_[0];
    if (max < kSize) {
      for (std::size_t i = 1; i < kSize; ++i)
        if (arr_[i] && arr_[i] < arr_[max_ind])
          max = arr_[max_ind = i];
    }
    return FromIndex(max_ind);
  }

  T FindMinContained() const {
    if (size_ == 0)
      return FromIndex(kSize);

    std::size_t min_ind = 0, min = arr_[0];
    if (min < kSize) {
      for (std::size_t i = 1; i < kSize; ++i)
        if (arr_[i] && arr_[i] < arr_[min_ind])
          min = arr_[min_ind = i];
    }
    return FromIndex(min_ind);
  }

  T FindMaxContained() const {
    if (size_ == 0)
      return FromIndex(kSize);

    std::size_t max_ind = 0, max = arr_[0];
    if (max < kSize) {
      for (std::size_t i = 1; i < kSize; ++i)
        if (arr_[i] && arr_[i] < arr_[max_ind])
          max = arr_[max_ind = i];
    }
    return FromIndex(max_ind);
  }

  template<typename U, typename V>
  V FindAverage() const {
    U sum = 0;
    for (std::size_t i = 1; i < kSize; ++i) {
      if (arr_[i])
        sum += arr_[i] * i;
    }
    return static_cast<V>(sum) / size_;
  }

  T FindMedian() const {
    if (size_ == 0)
      return FromIndex(kSize);

    std::size_t threshold = (size_ - 1) >> 1, cnt = 0;
    for (std::size_t i = 0; i < kSize; ++i) {
      if (arr_[i] && (cnt += arr_[i]) > threshold)
        return ToIndex(i);
    }
    assert(false);
  }

  template<typename U>
  T FindLeastSquaresFitBruteForce(
      U* least_square_sum = static_cast<U*>(NULL)) const {
    if (size_ == 0)
      return FromIndex(kSize);

    U lsf = ComputeDiffSquareSum(0);
    std::size_t lsf_ind = 0;
    for (std::size_t i = 1; i < kSize; ++i) {
      U dss = ComputeDiffSquareSum(static_cast<U>(i));
      if (dss < lsf) {
        lsf = dss;
        lsf_ind = i;
      }
    }
    if (least_square_sum != NULL)
      *least_square_sum = lsf;

    return FromIndex(lsf_ind);
  }

  // TODO fix a bug
  template<typename U>
  T FindLeastSquaresFitTernarySearch(
      U* least_square_sum = static_cast<U*>(NULL),
      const U& precision = static_cast<U>(0.25 - 1e-6)) const {
    if (size_ == 0)
      return FromIndex(kSize);

    U lo = static_cast<U>(0), hi = static_cast<U>(kSize - 1);
    while (hi - lo > precision) {
      U left_mid = (2 * lo + hi) / 3;
      U right_mid = (lo + 2 * hi) / 3;
      U left_mid_dss = ComputeDiffSquareSum(static_cast<U>(left_mid));
      U right_mid_dss = ComputeDiffSquareSum(static_cast<U>(right_mid));
      if (left_mid_dss < right_mid_dss)
        hi = right_mid;
      else
        lo = left_mid;
    }
    U mid = (lo + hi) / 2;
    if (least_square_sum != NULL)
      *least_square_sum = ComputeDiffSquareSum(mid);
    std::size_t lsf_ind = static_cast<U>(mid + 0.5);
    return FromIndex(lsf_ind);
  }

  template<typename U>
  T FindLeastSquaresFit(U* least_square_sum = static_cast<U*>(NULL),
      const U& precision = static_cast<U>(0.5 - 1e-6)) const {
    // TODO fixa bug in ternary search version
//    return WrapAround ? FindLeastSquaresFitBruteForce(least_square_sum)
//        : FindLeastSquaresFitTernarySearch(least_square_sum);
    return FindLeastSquaresFitBruteForce(least_square_sum);
  }

  template<bool biased, typename U>
  U ComputeStandardDeviation(const U& mean) const {
    if (size_ == 0)
      return FromIndex(kSize);

    return sqrt(ComputeDiffSquareSum(mean) / (biased ? size_ : size_ - 1));
  }

  friend std::ostream& operator<<(std::ostream& os, const SparseMultiset& s) {
    os << '[';
    std::size_t cnt = 0;
    for (std::size_t i = 0; i < kSize; ++i)
      if (s.arr_[i]) {
        cnt += s.arr_[i];
        os << ToIndex(i) << " (x" << s.arr_[i] << ')';
        if (cnt != s.size_) os << ", ";
      }
    os << ']';
    return os;
  }

  int size() const {
    return size_;
  }

  bool IsEmpty() const {
    return size_ == 0;
  }

private:
  struct NonZeroCountPredicate {
    bool operator()(std::size_t cnt) const {
      return cnt;
    }
  };

  static const std::size_t kSize = N;
  static const NonZeroCountPredicate kNonZeroCountPredicate;

  static inline std::size_t ToIndex(const T& t) {
    return static_cast<std::size_t>(t);
  }

  static inline T FromIndex(std::size_t index) {
    return static_cast<T>(index);
  }

  template<typename U>
  static U ComputeDiff(const U& x1, const U& x2) {
    return x1 < x2 ? x2 - x1 : x1 - x2;
  }

public:
  template<typename U>
  U ComputeDiffSquareSum(const U& x) const {
    U sum = 0, x_max = static_cast<U>(kSize);
    for (std::size_t i = 0; i < kSize; ++i)
      if (arr_[i]) {
        U d = ComputeDiff(x, static_cast<U>(i));
        if (WrapAround && x_max - d < d)
          d = x_max - d;
        sum += d * d * arr_[i];
      }
    return sum;
  }
private:

  std::size_t arr_[N];
  std::size_t size_;
};

#endif /* SPARSE_MULTISET_H_ */
