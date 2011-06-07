/*
 * data_builder_test.cpp
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
 * data_builder_test.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#include "../test.h"

#ifndef SKIP_TESTS

#include <gtest/gtest.h>

#include "../../src/ann/data_builder.h"
#include "../../src/ann/ann_vector_storage.h"

using namespace slesann::ann;

namespace {
  template<typename T>
  InputVector<T> ToIV(T a, T b) {
    InputVector<T> iv(2);
    iv[0] = a;
    iv[1] = b;
    return iv;
  }

  template<typename T>
  OutputVector<T> ToOV(T a) {
    OutputVector<T> ov(1);
    ov[0] = a;
    return ov;
  }
}

TEST(ann, data_builder) {
  DataBuilder db(2, 1);
  db.Append(ToIV(2, 3), ToOV(5));
  db.Append(ToIV(3, 4), ToOV(7));
  db.Append(ToIV(5, 1), ToOV(6));

  std::ofstream of;
  of.open("/tmp/db-test.txt");
  of << db;
  of.close();
}

TEST(ann, data_builder_speed01) {
  std::size_t in = 10000, on = 1000;
  DataBuilder db(in, on);
  for (int i = 0; i < 100; ++i) {
    db.Append(InputVector<int>(in), OutputVector<int>(on));
  }

  std::ofstream of;
  of.open("/tmp/db-speedtest.txt");
  of << db;
  of.close();
}

TEST(ann, vector_storage) {
  AnnVectorStorage avs("/tmp/avs");
  avs.Save(ToIV(2, 3), "two-three");
  avs.Save(ToIV(3, 4), "three-four");
  avs.Save(ToOV(5), "five");
  InputVector<int> iv1 = avs.LoadInputVector<int>("two-three"),
      iv2 = avs.LoadInputVector<int>("three-four");
  OutputVector<int> ov1 = avs.LoadOutputVector<int>("five");
  EXPECT_EQ(2, iv1[0]);
  EXPECT_EQ(3, iv1[1]);
  EXPECT_EQ(3, iv2[0]);
  EXPECT_EQ(4, iv2[1]);
  EXPECT_EQ(5, ov1[0]);
}

#endif
