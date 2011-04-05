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

#include <cstdio>

#include "gtest/gtest.h"
#include "boost/foreach.hpp"

#include "../test.h"
#include "../../src/matching/assignment.h"

using namespace slesann::matching;
using namespace slesann;

namespace slesann {

const double EPS = 1e-2;

void PrintConversion(double p) {
  int cost = ProbabilityToCost(p);
  double p2 = CostToProbability(cost);
  printf("%6.4lf -> %10d -> %6.4lf\n", p, cost, p2);
}

void CheckConversion(double p) {
  int cost = ProbabilityToCost(p);
  double p2 = CostToProbability(cost);
  EXPECT_NEAR(p, p2, EPS);
}

} // namespace slesann


TEST(assignment, convertToCost1) {
  for (int i = 0; i < 10; ++i) {
    double p = i / 10.0;
    CheckConversion(p);
    if (VERBOSITY)
      PrintConversion(p);
  }
}
