/*
 * libhungarian.cpp
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
 * libhungarian.cpp
 *
 *  Created on: Mar 2, 2011
 *      Author: losvald
 */

#include "gtest/gtest.h"

#include "../../src/matching/libhungarian/hungarian.h"

#include "libhungarian.h"

void DefaultRun(int** m, int rows, int cols) {
  hungarian_problem_t p;


  /* initialize the gungarian_problem using the cost matrix*/
  int matrix_size = hungarian_init(&p, m , rows, cols, HUNGARIAN_MODE_MAXIMIZE_UTIL) ;

  fprintf(stderr, "assignement matrix has a now a size %d rows and %d columns.\n\n",  matrix_size,matrix_size);

  /* some output */
  fprintf(stderr, "cost-matrix:");
  hungarian_print_costmatrix(&p);

  /* solve the assignement problem */
  hungarian_solve(&p);

  /* some output */
  fprintf(stderr, "assignment:");
  hungarian_print_assignment(&p);

  /* free used memory */
  hungarian_free(&p);
  free(m);
}

TEST(libhungarian, one) {
  /* an example cost matrix */
  int r[4*3] =  {  100, 1, 1,
                   100, 2, 2,
                   1, 0, 0,
                   0, 2, 0 };
  int** m = ArrayToMatrix(r,4,3);
  DefaultRun(m, 4, 3);
}

TEST(libhungarian, negativeCosts) {
  /* an example cost matrix */
  int r[2 * 2] =  {
                   1, -2,
                   -3, -7};
  int** m = ArrayToMatrix(r, 2, 2);
  DefaultRun(m, 2, 2);
}

TEST(libhungarian, random1) {
  int rows = 10, cols = 10;
  DefaultRun(RandomMatrix(rows, cols), rows, cols);
}

//TEST(libhungarian, speed1) {
//  int rows = 100, cols = 100;
//  DefaultRun(RandomMatrix(rows, cols), rows, cols);
//}
