/*
 * time_utils.h
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
 * time_utils.h
 *
 *  Created on: Apr 10, 2011
 *      Author: losvald
 */

#ifndef TIME_UTILS_H_
#define TIME_UTILS_H_

#include <string>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>

namespace slesann {

std::string PtimeToMillisecStrFromEpoch(
    const boost::posix_time::ptime& timestamp);

boost::posix_time::ptime MillisecStrFromEpochToPtime(
    const std::string& ms_from_epoch);

boost::posix_time::time_duration ToTimeDuration(double seconds);

}  // namespace slesann


#endif /* TIME_UTILS_H_ */
