/*
 * units.cpp
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
 * units.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: losvald
 */

#include "test.h"

#ifndef SKIP_TESTS

#include <boost/date_time/posix_time/posix_time.hpp>

#include "../src/units.h"
#include "../src/util/time_utils.h"


TEST(units, calculation) {
  using namespace slesann;
  using namespace boost::posix_time;
  using namespace boost::units;

  ptime cur_time = microsec_clock::local_time();
  units::Distance d = 705.3 * si::meter;
  units::Speed v = 10 * si::meter_per_second;

  quantity<si::time> f = d / v;

  units::UnitRealType q = f.value();

  time_duration td = ToTimeDuration(q);

  ptime t2 = cur_time - td;

  std::cout << "t: " << to_simple_string(cur_time) << std::endl;
  std::cout << "td: " << q << " s\n";
  std::cout << "td: " << to_simple_string(td) << " s\n";
  std::cout << "t - td: " << to_simple_string(t2) << std::endl;

  quantity<si::dimensionless, units::UnitRealType> dim1 = 10;
  quantity<si::time, units::UnitRealType> t1 = 20 * si::second;
  quantity<si::frequency, units::UnitRealType> freq = 10 /
      (td.total_seconds() * si::second);
//  dim1 / t1;
//  units::Frequency freq = dim1 / units::TimeDuration(td);

}

#endif
