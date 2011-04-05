/*
 * assignment.h
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
 * assignment.h
 *
 *  Created on: Mar 2, 2011
 *      Author: losvald
 */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include <vector>
#include <utility>

namespace slesann {

namespace matching {

static const int minCost = 1;

int ProbabilityToCost(double p);

double CostToProbability(int cost);

typedef std::pair<int, int> AssignmentPair;
typedef std::vector<AssignmentPair> Assignment;
typedef std::vector< std::vector<int> > CostMatrix;
typedef std::vector< std::vector<float> > ProbabilityMatrix;

Assignment MatchWithMaxProbability(const ProbabilityMatrix& probabilities);

void InitProbabilityMatrix(ProbabilityMatrix& probabilities,
                           int row_count, int column_count);

} // namespace matching

} // namespace slesann


#endif /* ASSIGNMENT_H_ */
