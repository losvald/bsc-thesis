/*
 * time_utils.cpp
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
 * time_utils.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: losvald
 */

#include "test.h"

#ifndef SKIP_TESTS

#include <iostream>
#include <map>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "../src/util/time_utils.h"

namespace epoch {

void CheckConversion(const std::string& s) {
  using namespace slesann;
  boost::posix_time::ptime p = MillisecStrFromEpochToPtime(s);
  std::string s2 = PtimeToMillisecStrFromEpoch(p);
  EXPECT_EQ(s, s2);
}

}  // namespace epoch

TEST(time_utils, epoch_conversion01) {
  using namespace slesann;
  using namespace boost::posix_time;
  using namespace boost::gregorian;

  ptime p(date(2003, Jun, 1));
  std::string s = PtimeToMillisecStrFromEpoch(p);
  ptime p2 = MillisecStrFromEpochToPtime(s);
  EXPECT_EQ(p, p2);

  std::map<ptime, int> m;
  m[p] = 2;
  m[p2] = 3;
  EXPECT_EQ(3, m[p]);

  epoch::CheckConversion("10101124");
}

#endif
