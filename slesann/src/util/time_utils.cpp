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

#include "time_utils.h"

#include <sstream>

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace slesann {

namespace {

static const boost::posix_time::ptime kEpoch(
    boost::gregorian::date(1970, boost::gregorian::Jan, 1));

boost::int64_t StrToInt64(const std::string& s) {
  boost::int64_t ret = 0;
  typedef std::string::const_iterator iterator;
  for(iterator it = s.begin(), end = s.end(); it != end; ++it)
    ret = 10 * ret + (*it - '0');
  return ret;
}

} // namespace

std::string PtimeToMillisecStrFromEpoch(
    const boost::posix_time::ptime& timestamp) {
  using boost::posix_time::ptime;
  using namespace boost::gregorian;
  std::stringstream ss;
  ss << (timestamp - kEpoch).total_milliseconds();
  return ss.str();
}

boost::posix_time::ptime MillisecStrFromEpochToPtime(
    const std::string& ms_from_epoch) {
  using boost::posix_time::ptime;
  return kEpoch + boost::posix_time::milliseconds(StrToInt64(ms_from_epoch));
}

boost::posix_time::time_duration ToTimeDuration(double seconds_val) {
  using namespace boost::posix_time;
  long sec_long = (long)(seconds_val / 1000 * 1000);
  long millisec_long = (long)(1000 * (seconds_val - sec_long));
//  std::cout << "(sec = " << sec_long << " | ms = " << millisec_long << std::endl;
  return seconds(sec_long) + milliseconds(millisec_long);
}

}  // namespace slesann
