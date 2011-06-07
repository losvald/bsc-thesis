/*
 * statistics.h
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
 * statistics.h
 *
 *  Created on: May 26, 2011
 *      Author: losvald
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <cmath>

#include <algorithm>
#include <map>
#include <vector>

#include <boost/foreach.hpp>

namespace slesann {

template<typename T>
double ComputeAverage(const std::vector<T>& v) {
  double sum = 0;
  BOOST_FOREACH(const T& x, v) {
    sum += x;
  }
  return sum / v.size();
}

template<typename T>
double ComputeStandardDeviation(const std::vector<T>& v, bool estimate,
                                double mean) {
  double diff_square_sum = 0;
  BOOST_FOREACH(const T& x, v) {
    diff_square_sum += (x - mean) * (x - mean);
  }
  return sqrt(diff_square_sum / (estimate ? v.size() - 1 : v.size()));
}

template<typename T>
class MeasurementGroup {
public:
  void Add(const T& x) {
    measurements_.push_back(x);
  }

  void Clear() {
    measurements_.clear();
  }

  double ComputeAverage() const {
    return slesann::ComputeAverage(measurements_);
  }

  double ComputeStandardDeviation(bool estimate = true) const {
    return slesann::ComputeStandardDeviation(measurements_, estimate,
                                             ComputeAverage());
  }

  double ComputeRelativeError() const {
    return 3 * ComputeStandardDeviation() / ComputeAverage();
  }

  double ComputeMaxDifference() const {
    return *std::max_element(measurements_.begin(), measurements_.end()) -
        *std::min_element(measurements_.begin(), measurements_.end());
  }

  const std::vector<T>& measurements() const {
    return measurements_;
  }

  const T& last() const {
    return measurements_.back();
  }

private:
  std::vector<T> measurements_;
};


typedef std::pair<std::string, MeasurementGroup<float> > MeasurementMapEntry;
typedef std::map<std::string, MeasurementGroup<float> > MeasurementMap;

const int kMeasureNameWidth = 30;

void PrintLastMeasurements(const MeasurementMap& mm);

void PrintStatistics(const MeasurementMap& mm, const std::string& name = "");

void PrintUniqueness(const MeasurementMap& mm_group_stddev,
                     const MeasurementMap& mm_group_avg,
                     const std::string& name = "uniqueness");

void DoAfterGroup(MeasurementMap* cur,
                  MeasurementMap* all,
                  MeasurementMap* group_avg,
                  MeasurementMap* group_stddev,
                  MeasurementMap* group_relerr,
                  MeasurementMap* group_maxdiff);


}  // namespace slesann

#endif /* STATISTICS_H_ */
