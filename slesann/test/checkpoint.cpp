/*
 * checkpoint.cpp
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
 * checkpoint.cpp
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#include "test.h"

#ifndef SKIP_TESTS

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "../src/checkpoint.h"
#include "../src/util/time_utils.h"

using namespace slesann;
using namespace boost::posix_time;

typedef Checkpoint<int> MyCheckpoint;

void Register(MyCheckpoint& cp, int vehicle, long timestamp) {
  std::stringstream ss; ss << timestamp;
  cp.Register(vehicle, MillisecStrFromEpochToPtime(ss.str()));
}

ptime PTime(long timestamp) {
  std::stringstream ss; ss << timestamp;
  return MillisecStrFromEpochToPtime(ss.str());
}

TEST(checkpoint, register_01) {
  MyCheckpoint cp(0);

  EXPECT_EQ(0, cp.Get(PTime(0), PTime(50)).size());

  Register(cp, 1, 10);

  EXPECT_EQ(1, cp.Get(PTime(0), PTime(50)).size());
  EXPECT_EQ(1, cp.Get(PTime(8), PTime(11)).size());
  EXPECT_EQ(0, cp.Get(PTime(0), PTime(9)).size());
  EXPECT_EQ(0, cp.Get(PTime(12), PTime(100)).size());
  EXPECT_EQ(0, cp.Count(PTime(12), PTime(100)));

  Register(cp, 2, 15);
  EXPECT_EQ(2, cp.Get(PTime(0), PTime(50)).size());
  EXPECT_EQ(2, cp.Count(PTime(5), PTime(20)));
  EXPECT_EQ(1, cp.Get(PTime(8), PTime(11)).size());
  EXPECT_EQ(1, cp.Count(PTime(8), PTime(11)));
  EXPECT_EQ(0, cp.Get(PTime(0), PTime(9)).size());
  EXPECT_EQ(1, cp.Get(PTime(0), PTime(12)).size());
  EXPECT_EQ(1, cp.Get(PTime(12), PTime(100)).size());
}

TEST(checkpoint, register_02) {
  MyCheckpoint cp(0);
  cp.Register(1, ptime(neg_infin));
  cp.Register(2, ptime(neg_infin));

  EXPECT_EQ(2, cp.Count(neg_infin, pos_infin));
}

TEST(checkpoint, forget_01) {
  MyCheckpoint cp(1);

  Register(cp, 1, 5);
  Register(cp, 2, 15);
  Register(cp, 3, 23);
  Register(cp, 4, 32);
  Register(cp, 5, 47);
  EXPECT_EQ(5, cp.Count(PTime(0), PTime(50)));

  EXPECT_EQ(0, cp.ForgetBefore(PTime(3)));
  EXPECT_EQ(5, cp.Count(PTime(0), PTime(50)));
  EXPECT_EQ(2, cp.Count(PTime(30), PTime(50)));

  // 5 15 23 32 47 -> 23 32 47
  EXPECT_EQ(2, cp.ForgetBefore(PTime(16)));
  EXPECT_EQ(3, cp.Count(PTime(0), PTime(50)));
  EXPECT_EQ(2, cp.Count(PTime(30), PTime(50)));

  // 23 32 47 -> 23 32 47
  EXPECT_EQ(0, cp.ForgetBefore(PTime(22)));
  EXPECT_EQ(3, cp.Count(PTime(0), PTime(50)));
  EXPECT_EQ(2, cp.Count(PTime(30), PTime(50)));

  // 23 32 47 -> 32 47
  EXPECT_EQ(1, cp.ForgetBefore(PTime(23)));
  EXPECT_EQ(2, cp.Count(PTime(0), PTime(50)));
  EXPECT_EQ(2, cp.Count(PTime(30), PTime(50)));
}

TEST(checkpoint, forget_02) {
  MyCheckpoint cp(2);

  Register(cp, 1, 3);
  Register(cp, 2, 52);
  Register(cp, 3, 6346);
  Register(cp, 4, 12425);

  EXPECT_EQ(4, cp.ForgetBefore(pos_infin));
  EXPECT_EQ(0, cp.Count(neg_infin, pos_infin));
}



TEST(checkpoint, pass_freq_01) {
  MyCheckpoint cp(5);
  Register(cp, 1, 5);
  Register(cp, 2, 15);
  Register(cp, 3, 23);
  Register(cp, 4, 32);
  Register(cp, 5, 47);

  const double EPS = 1e-2;
  time_duration td = PTime(20) - PTime(0);
  EXPECT_NEAR(100, cp.VehiclePassFrequency(PTime(0), PTime(20)).value(), EPS);
  EXPECT_NEAR(0, cp.VehiclePassFrequency(PTime(16), PTime(22)).value(), EPS);
  EXPECT_NEAR(5, cp.VehiclePassFrequency(PTime(0), PTime(1000)).value(), EPS);
}

#endif
