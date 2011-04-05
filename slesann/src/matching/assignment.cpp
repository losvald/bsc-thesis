/*
 * assignment.cpp
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
 * assignment.cpp
 *
 *  Created on: Mar 2, 2011
 *      Author: losvald
 */

#include <cstdio> // XXX

#include <algorithm>
#include <cmath>
#include <cassert>

#include "assignment.h"
#include "libhungarian/hungarian.h"

namespace slesann {

namespace matching {

const double PROB_FACTOR = 1000;
const double COST_FACTOR = 1000;

int ProbabilityToCost(double p) {
  return log(std::max(1.0, p * PROB_FACTOR)) * COST_FACTOR;
}

double CostToProbability(int cost) {
  return exp(cost / COST_FACTOR) / PROB_FACTOR;
}

void InitProbabilityMatrix(ProbabilityMatrix& probabilities,
                           int row_count, int column_count) {
  probabilities.resize(row_count);
  std::fill(probabilities.begin(), probabilities.end(),
            std::vector<float>(row_count, 0.0f));
}

Assignment ToAssignment(const hungarian_problem_t& p) {
  Assignment assignment;
  assignment.reserve(std::min(p.num_rows, p.num_cols));
  for (int row = p.num_rows - 1; row >= 0; --row)
    for (int col = p.num_cols - 1; col >= 0; --col)
      if (p.assignment[row][col]) {
        assignment.push_back(AssignmentPair(row, col));
        break;
      }
  assert(assignment.size() == assignment.capacity());
  return assignment;
}

std::size_t RowCount(const ProbabilityMatrix& probabilities) {
  return probabilities.size();
}

std::size_t columnCount(const ProbabilityMatrix& probabilities) {
  return RowCount(probabilities) == 0 ? 0 : probabilities[0].size();
}

Assignment MatchWithMaxProbability(const ProbabilityMatrix& probabilities) {
  std::size_t rows = RowCount(probabilities);
  std::size_t cols = columnCount(probabilities);
  int** m = new int*[rows];
  for(std::size_t i = 0; i < rows; i++) {
    m[i] = new int[cols];
    for(std::size_t j = 0; j < cols; j++)
      m[i][j] = ProbabilityToCost(probabilities[i][j]);
  }

  /* initialize the hungarian_problem using the cost matrix */
  hungarian_problem_t p;
  int matrix_size = hungarian_init(&p, m , rows, cols, HUNGARIAN_MODE_MAXIMIZE_UTIL) ;

  fprintf(stderr, "assignement matrix has a now a size %d rows and %d columns.\n\n",
          matrix_size, matrix_size);

  /* some output */
  fprintf(stderr, "cost-matrix:");
  hungarian_print_costmatrix(&p);

  /* solve the assignement problem */
  hungarian_solve(&p);

  /* some output */
  fprintf(stderr, "assignment:");
  hungarian_print_assignment(&p);

  /* compute result and free used memory */
  Assignment result = ToAssignment(p);
  hungarian_free(&p);
  free(m);

  return result;
}

} // namespace matching

} // namespace slesann




