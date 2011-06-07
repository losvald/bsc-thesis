/*
 * statistics.cpp
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
 * statistics.cpp
 *
 *  Created on: May 26, 2011
 *      Author: losvald
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "statistics.h"

namespace slesann {

void PrintLastMeasurements(const MeasurementMap& mm) {
  BOOST_FOREACH(const MeasurementMapEntry& e, mm) {
    std::cout << std::left << std::setw(kMeasureNameWidth) << e.first <<
        std::fixed << std::right << std::setw(20) <<
        std::setprecision(4) << e.second.last() << std::endl;
  }
}

void PrintStatistics(const MeasurementMap& mm, const std::string& name) {
  const int kAvgWidth = 12;
  const int kStdDevWidth = 12;
  const int kRelErrWidth = 12;
  const int kMaxDiffWidth = 14;
  std::cout << std::left << std::setw(kMeasureNameWidth) << name <<
      std::right << std::setw(kAvgWidth) << "average" <<
      std::right << std::setw(kStdDevWidth) << "std. dev." <<
      std::right << std::setw(kRelErrWidth) << "rel. error" <<
      std::right << std::setw(kMaxDiffWidth) << "max. diff." << std::endl;
  BOOST_FOREACH(const MeasurementMapEntry& e, mm) {
    std::cout << std::left << std::setw(kMeasureNameWidth) << e.first <<
        std::fixed << std::right << std::setw(kAvgWidth) <<
        std::setprecision(4) << e.second.ComputeAverage() <<
        std::fixed << std::right << std::setw(kStdDevWidth) <<
        std::setprecision(4) << e.second.ComputeStandardDeviation() <<
        std::fixed << std::right << std::setw(kRelErrWidth) <<
        std::setprecision(4) << e.second.ComputeRelativeError() <<
        std::right << std::setw(kMaxDiffWidth) <<
        std::setprecision(4) << e.second.ComputeMaxDifference() <<
        std::endl;
  }
}

void PrintUniqueness(const MeasurementMap& mm_group_stddev,
                     const MeasurementMap& mm_group_avg,
                     const std::string& name) {
  const int kStdDevRatioWidth = 30;
  std::cout << std::left << std::setw(kMeasureNameWidth) << name <<
        std::right << std::setw(kStdDevRatioWidth) <<
        "stddev(out-group) / stddev(in-group)" << std::endl;
  BOOST_FOREACH(const MeasurementMapEntry& e, mm_group_avg) {
    double uniqueness = e.second.ComputeStandardDeviation(true) /
        mm_group_stddev.at(e.first).ComputeAverage();
    std::cout << std::left << std::setw(kMeasureNameWidth) << e.first <<
        std::fixed << std::right << std::setw(kStdDevRatioWidth) <<
        std::setprecision(4) << uniqueness << std::endl;
  }
}

void DoAfterGroup(MeasurementMap* cur,
                  MeasurementMap* all,
                  MeasurementMap* group_avg,
                  MeasurementMap* group_stddev,
                  MeasurementMap* group_relerr,
                  MeasurementMap* group_maxdiff) {
  BOOST_FOREACH(const MeasurementMapEntry& e, *cur) {
    const std::string& prop = e.first;
    (*group_avg)[prop].Add(e.second.ComputeAverage());
    (*group_stddev)[prop].Add(e.second.ComputeStandardDeviation());
    (*group_relerr)[prop].Add(e.second.ComputeRelativeError());
    (*group_maxdiff)[prop].Add(e.second.ComputeMaxDifference());
    BOOST_FOREACH(const float m, e.second.measurements()) {
      (*all)[prop].Add(m);
    }
  }
  cur->clear();
}

}  // namespace slesann
