/*
 * libhungarian.h
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
 * libhungarian.h
 *
 *  Created on: Mar 2, 2011
 *      Author: losvald
 */

#ifndef TEST_LIBHUNGARIAN_H_
#define TEST_LIBHUNGARIAN_H_

#include "boost/random.hpp"

int Random(int from, int to) {
  static boost::mt19937 gen;

  boost::uniform_int<> dist(from, to);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
  return die();
}

int** ArrayToMatrix(int* m, int rows, int cols) {
  int i,j;
  int** r;
  r = (int**)calloc(rows,sizeof(int*));
  for (i = 0; i < rows; ++i) {
    r[i] = (int*)calloc(cols, sizeof(int));
    for (j = 0; j < cols; ++j)
      r[i][j] = m[i * cols + j];
  }
  return r;
}

int** RandomMatrix(int rows, int cols) {
  int n = rows * cols;
  int* arr = new int[n];
  for (int i = 0; i < n; ++i)
    arr[i] = Random(0, 100);
  return ArrayToMatrix(arr, rows, cols);
}

#endif /* TEST_LIBHUNGARIAN_H_ */
