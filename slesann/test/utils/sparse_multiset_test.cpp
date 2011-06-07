/*
 * sparse_multiset_test.cpp
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
 * sparse_multiset_test.cpp
 *
 *  Created on: May 28, 2011
 *      Author: losvald
 */

#include "../test.h"

//#ifndef SKIP_TESTS

#include <cstdlib>

//#include <boost/static_assert.hpp>

#include "../../src/util/sparse_multiset.h"

template<typename T, int N> SparseMultiset<T, N> Generate(int n) {
  SparseMultiset<T, N> s;
  for (int i = 0; i < n; ++i) {
    s.Add(rand() % N);
  }
  return s;
}

//template<typename T>
//struct Foo {
//
//  template<typename U> bool Bar() const {
//    U a = 0, b = 1;
//    return a < b;
//  }
//
//  template<typename U> bool Bar2(const U* u) {
//    U a = 0, b = 1;
//    return a < b;
//  }
//
//};

template<typename T, int N>
void CheckLeastSquaresFit(const SparseMultiset<T, N>& s) {
  std::cout << "LSF: " << s << std::endl;
  std::size_t exp, act;
  EXPECT_EQ(exp = s.FindLeastSquaresFitBruteForce(static_cast<float*>(NULL)),
            act = s.FindLeastSquaresFitTernarySearch(static_cast<float*>(NULL),
                                                     0.05f));
}

template<typename T, int N>
void TestLeastSquaresFitTernarySearch(int n, int max_itr) {
  for (int itr = 0; itr < max_itr; ++itr) {
    SparseMultiset<T, N> s = Generate<T, N>(n);
    CheckLeastSquaresFit(s);
  }
}

TEST(sparse_multiset, average01) {
  SparseMultiset<int, 7> s;
  s.Add(5); s.Add(6); s.Add(2);
  EXPECT_EQ(s.size(), 3);
  EXPECT_NEAR((s.FindAverage<float, float>()), 13.f / 3, 1e-6);
  s.Add(5);
  EXPECT_EQ(s.size(), 4);
  EXPECT_NEAR((s.FindAverage<float, float>()), 4.5, 1e-6);
  s.Remove(5);
  EXPECT_EQ(s.size(), 3);
  EXPECT_NEAR((s.FindAverage<int, float>()), 13.f / 3, 1e-6);
}

TEST(sparse_multiset, average02) {
  SparseMultiset<char, 10> s;
  s.AddMultiple(0, 2);
  s.AddMultiple(1, 3);
  s.AddMultiple(2, 1);
  s.AddMultiple(3, 2);
  s.AddMultiple(4, 2);
  EXPECT_NEAR(1.9, (s.FindAverage<int, float>()), 1e-6);
}

TEST(sparse_multiset, median01) {
  SparseMultiset<long, 10> s;
  s.AddMultiple(0, 4);
  s.AddMultiple(1, 1);
  s.AddMultiple(2, 1);
  s.AddMultiple(3, 2);
  s.AddMultiple(4, 2);
  EXPECT_EQ(1, s.FindMedian());
  s.Remove(1);
  EXPECT_EQ(2, s.FindMedian());
  s.Add(0);
  std::cout << s << std::endl;
  EXPECT_EQ(0, s.FindMedian());
}

TEST(sparse_multiset, median02) {
  SparseMultiset<int, 1000> s;
  s.Add(500);
  s.Add(900);
  EXPECT_EQ(500, s.FindMedian());
  s.Add(899);
  EXPECT_EQ(899, s.FindMedian());
}

TEST(sparse_multiset, stddev01) {
  SparseMultiset<int, 10> s;
  s.Add(1); s.Add(5); s.Add(6);
  EXPECT_NEAR(2.64575, s.ComputeStandardDeviation<false>(
      s.FindAverage<int, float>()), 1e-5);
  EXPECT_NEAR(2.16025, s.ComputeStandardDeviation<true>(
        s.FindAverage<int, float>()), 1e-5);
}

TEST(sparse_multiset, LeastSquareFitTernarySearch01) {
  SparseMultiset<int, 10> s;
  s.Add(9); s.Add(5); s.Add(5); s.Add(9); s.Add(7);
  EXPECT_EQ(7, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_EQ(7, s.FindLeastSquaresFitTernarySearch<float>());
  s.Add(8); s.Add(6);
  EXPECT_EQ(7, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_EQ(7, s.FindLeastSquaresFitTernarySearch<float>());
}

TEST(sparse_multiset, LeastSquareFitTernarySearch02) {
  SparseMultiset<int, 10> s;
  s.AddMultiple(0, 2);
  s.AddMultiple(1, 3);
  s.AddMultiple(2, 1);
  s.AddMultiple(3, 2);
  s.AddMultiple(4, 2);
  EXPECT_EQ(2, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_EQ(2, s.FindLeastSquaresFitTernarySearch<float>());
  s.Add(8);
  EXPECT_EQ(2, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_EQ(2, s.FindLeastSquaresFitTernarySearch<float>());

  s.Add(3);
  EXPECT_EQ(2, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_NEAR(2, s.FindLeastSquaresFitTernarySearch<float>(),
                1.01f);
  s.Add(6);
  EXPECT_EQ(3, s.FindLeastSquaresFitBruteForce<float>());
  EXPECT_EQ(3, s.FindLeastSquaresFitTernarySearch<float>());
}

TEST(sparse_multiset, DISABLE_LeastSquareFitTernarySearch03) {
  TestLeastSquaresFitTernarySearch<int, 5>(10, 100);
}

TEST(sparse_multiset, LeastSquareFitWrapAround01) {
  SparseMultiset<int, 10, true> s;
  int diff_square_sum;

  s.AddMultiple(1, 2);
  s.AddMultiple(9, 2);
  s.Add(5);
  EXPECT_EQ(1, s.FindLeastSquaresFit<int>(&diff_square_sum));
  EXPECT_EQ(24, diff_square_sum);

  s.Remove(5);
  s.Add(6);
  EXPECT_EQ(9, s.FindLeastSquaresFit<int>(&diff_square_sum));
  EXPECT_EQ(17, diff_square_sum);

  s.AddMultiple(1, 5);
  s.AddMultiple(9, 5);
  EXPECT_EQ(0, s.FindLeastSquaresFit<int>());
}


//#endif

