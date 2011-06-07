/*
 * speed_limit_detector.h
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
 * speed_limit_detector.h
 *
 *  Created on: Apr 5, 2011
 *      Author: losvald
 */

#ifndef SPEED_LIMIT_DETECTOR_H_
#define SPEED_LIMIT_DETECTOR_H_

#include <vector>
#include <algorithm>
#include <sstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/smart_ptr.hpp>

#include "base.h"
#include "checkpoint.h"
#include "units.h"
#include "util/time_utils.h"

#include "matching/assignment.h"

#include "descriptor_matcher.h"

#include "violator_info.h"

namespace slesann {

template< class T, class TAlloc = DefaultDescriptorAllocator<T>,
    class TDealloc = DefaultDescriptorDeallocator<T> >
class SpeedLimitDetector {
public:
  typedef boost::shared_ptr<SpeedLimitDetector> SharedPtr;
  typedef Checkpoint<T, TAlloc, TDealloc> CheckpointType;
  typedef ViolatorInfo<T> ViolatorInfoType;
  typedef DescriptorMatcher<T> DescriptorMatcherType;
  typedef typename DescriptorMatcher<T>::SharedPtr DescriptorMatcherSharedPtr;

  SpeedLimitDetector(CheckpointId start_checkpoint_id,
                     CheckpointId end_checkpoint_id,
                     const units::Distance& checkpoint_distance,
                     const units::Speed& max_speed,
                     const units::Speed& min_speed,
                     DescriptorMatcherSharedPtr matcher);


  std::vector<ViolatorInfoType> DetectViolators(const boost::posix_time::ptime& timestamp);

  /**
   * Returns the difference in time between vehicles
   * moving at constant speeds min_speed and max_speed, accordingly.
   *
   * @return the time interval in seconds
   */
  units::TimeDuration TimeWindowDuration() const;

  /**
   * Returns the difference in time between vehicles
   * moving at constant speeds min_speed and max_speed, accordingly.
   *
   * @return the time interval in seconds
   */
  units::UnitRealType TimeWindowVehicleCount(
      const boost::posix_time::ptime& timestamp) const;

  const units::Distance& checkpoint_distance() const;
  const units::Speed& min_speed() const;
  const units::Speed& max_speed() const;
  const CheckpointType& start_checkpoint() const;
  const CheckpointType& end_checkpoint() const;
  CheckpointType& start_checkpoint();
  CheckpointType& end_checkpoint();

private:

  boost::posix_time::ptime EarliestAtStart(const boost::posix_time::ptime& timestamp);
  boost::posix_time::ptime LatestAtStart(const boost::posix_time::ptime& timestamp);

  CheckpointType start_checkpoint_;
  CheckpointType end_checkpoint_;

  units::Distance checkpoint_distance_;
  units::Speed min_speed_;
  units::Speed max_speed_;

  DescriptorMatcherSharedPtr matcher_;
};

template<class T, class TAlloc, class TDealloc>
SpeedLimitDetector<T, TAlloc, TDealloc>::SpeedLimitDetector(
    CheckpointId start_checkpoint_id,
    CheckpointId end_checkpoint_id,
    const units::Distance& checkpoint_distance,
    const units::Speed& max_speed,
    const units::Speed& min_speed,
    DescriptorMatcherSharedPtr matcher)
    : start_checkpoint_(start_checkpoint_id),
      end_checkpoint_(end_checkpoint_id),
      checkpoint_distance_(checkpoint_distance),
      min_speed_(min_speed),
      max_speed_(max_speed),
      matcher_(matcher) {

}

template<class T, class TAlloc, class TDealloc>
std::vector<typename SpeedLimitDetector<T, TAlloc, TDealloc>::ViolatorInfoType>
SpeedLimitDetector<T, TAlloc, TDealloc>::DetectViolators(
    const boost::posix_time::ptime& timestamp) {

  EZLOGGERSTREAM << "Detecting violators at " << timestamp << std::endl;

  std::vector<ViolatorInfoType> violators;

  const boost::posix_time::ptime& end_from = timestamp;
  const boost::posix_time::ptime end_to = boost::posix_time::pos_infin;
  std::vector<T> end_desc = end_checkpoint_.Get(end_from, end_to);

  EZDBGONLYLOGGERSTREAM2(std::cerr) << "End checkpoint time window: [" << end_from <<
      ", " << end_to << std::endl;
#ifdef EZLOGGER_IMPLEMENT_DEBUGLOGGING
  BOOST_FOREACH(const T& desc, end_desc) {
    EZDBGONLYLOGGERSTREAM2(std::cerr) << desc << std::endl;
  }
#endif

  if (end_desc.empty()) {
    EZLOGGERSTREAM2(std::cerr) << "Nothing to detect - no vehicle passed end\n";
    return violators;
  }

  const boost::posix_time::ptime start_from = EarliestAtStart(timestamp);
  const boost::posix_time::ptime start_to = LatestAtStart(timestamp);
  std::vector<T> start_desc = start_checkpoint_.Get(start_from, start_to);

  EZDBGONLYLOGGERSTREAM2(std::cerr) << "Start checkpoint time window: [" << start_from <<
      ", " << start_to << std::endl;
#ifdef EZLOGGER_IMPLEMENT_DEBUGLOGGING
  BOOST_FOREACH(const T& desc, start_desc) {
    EZDBGONLYLOGGERSTREAM2(std::cerr) << desc << std::endl;
  }
#endif

  if (start_desc.empty()) {
    EZLOGGERSTREAM2(std::cerr) << "Nothing to detect - no vehicle passed start\n";
    return violators;
  }

  typedef typename DescriptorMatcherType::MatchedPair MatchedPair;
  typedef std::pair<MatchedPair, float> MatchEntry;
  std::vector<MatchEntry> matches = matcher_->Match(start_desc, end_desc);

  BOOST_FOREACH(const MatchEntry& e, matches) {
    const MatchedPair& matched_pair = e.first;
    const float prob = e.second;
    std::stringstream ss;
    ss << "Match " << prob << " " << matched_pair.first <<
        " -> " << matched_pair.second << std::endl;
    EZLOGGER(ss.str());
    //EZLOGGERSTREAM2(std::cerr) << ss.str();
  }

  // TODO
}

template<class T, class TAlloc, class TDealloc>
units::UnitRealType SpeedLimitDetector<T, TAlloc, TDealloc>::
TimeWindowVehicleCount(const boost::posix_time::ptime& timestamp) const {
  units::TimeDuration time_wnd_duration = TimeWindowDuration();
  boost::posix_time::time_duration duration(time_wnd_duration);
  const boost::posix_time::ptime from = timestamp - duration;
  const boost::posix_time::ptime& to = timestamp;
  return start_checkpoint_.Count(from, to);
}

template<class T, class TAlloc, class TDealloc>
units::TimeDuration SpeedLimitDetector<T, TAlloc, TDealloc>::TimeWindowDuration() const {
  return checkpoint_distance_ * (units::kOne / min_speed_) * (units::kOne / max_speed_);
}

template<class T, class TAlloc, class TDealloc>
const units::Speed&
SpeedLimitDetector<T, TAlloc, TDealloc>::min_speed() const {
  return min_speed_;
}

template<class T, class TAlloc, class TDealloc>
const units::Speed&
SpeedLimitDetector<T, TAlloc, TDealloc>::max_speed() const {
  return max_speed_;
}

template<class T, class TAlloc, class TDealloc>
const units::Distance&
SpeedLimitDetector<T, TAlloc, TDealloc>::checkpoint_distance() const {
  return checkpoint_distance_;
}

// private methods

template<class T, class TAlloc, class TDealloc>
boost::posix_time::ptime
SpeedLimitDetector<T, TAlloc, TDealloc>::EarliestAtStart(
    const boost::posix_time::ptime& timestamp) {
  return timestamp - ToTimeDuration(
      (checkpoint_distance_ / max_speed_).value());
}

template<class T, class TAlloc, class TDealloc>
boost::posix_time::ptime
SpeedLimitDetector<T, TAlloc, TDealloc>::LatestAtStart(
    const boost::posix_time::ptime& timestamp) {
  return timestamp - ToTimeDuration(
      (checkpoint_distance_ / min_speed_).value());
}

template<class T, class TAlloc, class TDealloc>
const typename SpeedLimitDetector<T, TAlloc, TDealloc>::CheckpointType&
SpeedLimitDetector<T, TAlloc, TDealloc>::start_checkpoint() const {
  return start_checkpoint_;
}

template<class T, class TAlloc, class TDealloc>
const typename SpeedLimitDetector<T, TAlloc, TDealloc>::CheckpointType&
SpeedLimitDetector<T, TAlloc, TDealloc>::end_checkpoint() const {
  return end_checkpoint_;
}

template<class T, class TAlloc, class TDealloc>
typename SpeedLimitDetector<T, TAlloc, TDealloc>::CheckpointType&
SpeedLimitDetector<T, TAlloc, TDealloc>::start_checkpoint() {
  return start_checkpoint_;
}

template<class T, class TAlloc, class TDealloc>
typename SpeedLimitDetector<T, TAlloc, TDealloc>::CheckpointType&
SpeedLimitDetector<T, TAlloc, TDealloc>::end_checkpoint() {
  return end_checkpoint_;
}

} // namespace slesann

#endif /* SPEED_LIMIT_DETECTOR_H_ */
