/*
 * matcher.h
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
 * matcher.h
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#ifndef MATCHER_H_
#define MATCHER_H_

#include <vector>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#include "../checkpoint.h"
#include "../units.h"

namespace slesann {



template< class T, class TAlloc = DefaultDescriptorAllocator<T>,
    class TDealloc = DefaultDescriptorDeallocator<T> >
class SpeedLimitEnforcer {

public:
  typedef Checkpoint<T, TAlloc, TDealloc> CheckpointType;

  SpeedLimitEnforcer(CheckpointId start_checkpoint_id,
                     CheckpointId end_checkpoint_id,
                     const units::Distance& checkpoint_distance,
                     const units::Speed& max_speed,
                     const units::Speed& min_speed);


  std::vector<T> DetectViolators(const boost::posix_time::ptime& timestamp);

  const units::Distance& checkpoint_distance() const;
  const units::Speed& min_speed() const;
  const units::Speed& max_speed() const;

private:

  CheckpointType start_checkpoint_;
  CheckpointType end_checkpoint_;

  units::Distance checkpoint_distance_;
  units::Speed min_speed_;
  units::Speed max_speed_;
};

template<class T, class TAlloc, class TDealloc>
SpeedLimitEnforcer<T, TAlloc, TDealloc>::SpeedLimitEnforcer(
    CheckpointId start_checkpoint_id,
    CheckpointId end_checkpoint_id,
    const units::Distance& checkpoint_distance,
    const units::Speed& max_speed,
    const units::Speed& min_speed)
    : start_checkpoint_(start_checkpoint_id),
      end_checkpoint_(end_checkpoint_id),
      checkpoint_distance_(checkpoint_distance),
      min_speed_(min_speed),
      max_speed_(max_speed) {

}

template<class T, class TAlloc, class TDealloc>
std::vector<T> SpeedLimitEnforcer<T, TAlloc, TDealloc>::DetectViolators(
    const boost::posix_time::ptime& timestamp) {
  // TODO

}

template<class T, class TAlloc, class TDealloc>
const units::Speed&
SpeedLimitEnforcer<T, TAlloc, TDealloc>::min_speed() const {
  return min_speed_;
}

template<class T, class TAlloc, class TDealloc>
const units::Speed&
SpeedLimitEnforcer<T, TAlloc, TDealloc>::max_speed() const {
  return max_speed_;
}

template<class T, class TAlloc, class TDealloc>
const units::Distance&
SpeedLimitEnforcer<T, TAlloc, TDealloc>::checkpoint_distance() const {
  return checkpoint_distance_;
}


} // namespace slesann

#endif /* MATCHER_H_ */
