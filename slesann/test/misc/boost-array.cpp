/*
 * boost-array.cpp
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
 * boost-array.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: losvald
 */

#include "gtest/gtest.h"

#include "boost/array.hpp"

template<typename T, std::size_t RowCount, std::size_t ColumnCount>
boost::array< boost::array<T, ColumnCount>, RowCount> Create2d(
    std::size_t row_count, std::size_t column_count) {
  return boost::array< boost::array<T, ColumnCount>, RowCount>();
}

TEST(boost_array, one) {
  using namespace boost;
  array<int, 5> a = {{1, 2, 3, 4, 5}};

  EXPECT_EQ(1, a.front());
  EXPECT_EQ(2, a.at(1));
  EXPECT_EQ(3, a[2]);
}
