/*
 * checkpoint.h
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
 * checkpoint.h
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#ifndef CHECKPOINT_H_
#define CHECKPOINT_H_

#include <vector>
#include <map>
#include <sstream>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#include "base.h"
#include "types.h"
#include "descriptor_memory.h"
#include "vehicle_descriptor_handler.h"
#include "util/time_utils.h"

#include "units.h"

namespace slesann {

typedef int CheckpointId;

template< class T, class TAlloc = DefaultDescriptorAllocator<T>,
    class TDealloc = DefaultDescriptorDeallocator<T> >
class Checkpoint : public VehicleDescriptorHandler<T> {
public:
  typedef T VehicleDescriptorType;

  /**
   * Construct a checkpoint with the specified identifier.
   *
   * @param id the unique identifier
   */
  Checkpoint(CheckpointId id);

  /**
   * Registers that the specified vehicle has passed the checkpoint.
   *
   * @param vehicle_descriptor the descriptor describing the vehicle
   * @param timestamp the time at which the vehicle passed the checkpoint
   */
  void Register(const T& vehicle_descriptor,
                          const boost::posix_time::ptime& timestamp);

//  /**
//   * Clears any information about the vehicle passed
//   *
//   * @param vehicle_descriptor the descriptor describing the vehicle
//   * @return true if the vehicle is actually registered, false otherwise
//   */
//  bool Forget(VehicleImageId vehicle_id);

  /**
   * Clears descriptors of the vehicles which passed the checkpoint earlier
   * than specified by the timestamp.
   *
   * @param timestamp the timestamp
   * @return the number of descriptors forgotten
   */
  std::size_t ForgetBefore(const boost::posix_time::ptime& timestamp);

  /**
   * Returns the list of descriptors belonging to the time window [form, end).
   *
   * @param from starting time
   * @param end ending time
   * @return the list of descriptors
   */
  std::vector<T> Get(const boost::posix_time::ptime& from,
      const boost::posix_time::ptime& to) const;

  /**
   * Returns the number of vehicle descriptors belonging to
   * the time window [form, end).
   *
   * @param from starting time
   * @param end ending time
   * @return the number of descriptors
   */
  std::size_t Count(const boost::posix_time::ptime& from,
      const boost::posix_time::ptime& to) const;

  void HandleVehicleDescriptor(const VehicleDescriptorEvent<T>& e);

  /**
   * Returns the frequency at which vehicles were passing
   * through this checkpoint in the time period
   * [from, to].
   *
   * @return the number of vehicles per second
   */
  units::Frequency VehiclePassFrequency(
      const boost::posix_time::ptime& from,
      const boost::posix_time::ptime& to) const;

  /**
   * Returns the identifier of this checkpoint.
   *
   * @return the unique identifier
   */
  CheckpointId id() const;

private:

  CheckpointId id_;
  std::multimap<boost::posix_time::ptime, T> vehicles_by_time_;
  TAlloc desc_aloc_;
  TDealloc desc_dealoc_;
};

template<class T, class TAlloc, class TDealloc>
Checkpoint<T, TAlloc, TDealloc>::
Checkpoint(CheckpointId id) : id_(id) {
}

template<class T, class TAlloc, class TDealloc>
CheckpointId Checkpoint<T, TAlloc, TDealloc>::id() const {
  return id_;
}

template<class T, class TAlloc, class TDealloc>
void Checkpoint<T, TAlloc, TDealloc>::
Register(const T& vehicle, const boost::posix_time::ptime& timestamp) {
  namespace pt = boost::posix_time;
  desc_aloc_(vehicle);
  vehicles_by_time_.insert(std::make_pair(timestamp, vehicle));

  std::stringstream ss;
  ss << id_ << ": " << PtimeToMillisecStrFromEpoch(timestamp) << " - " <<
      vehicle << std::endl;
  EZLOGGER(ss.str());
  std::string s(ss.str());
  EZLOGGERSTREAM2(std::cerr) << s;
}

//template<class T, class TAlloc, class TDealloc>
//bool Checkpoint<T, TAlloc, TDealloc>::
//Forget(VehicleImageId vehicle_id) {
//  BOOST_AUTO(it, desc_by_id_.find(vehicle_id));
//  if (it != desc_by_id_.end()) {
//    desc_dealoc_(*it);
//    return true;
//  }
//  return false;
//}

template<class T, class TAlloc, class TDealloc>
std::size_t Checkpoint<T, TAlloc, TDealloc>::
ForgetBefore(const boost::posix_time::ptime& timestamp) {
  typedef BOOST_TYPEOF(vehicles_by_time_.begin()) iterator;
  std::size_t count = 0;
  iterator first = vehicles_by_time_.begin();
  iterator last = vehicles_by_time_.upper_bound(timestamp);
  for (iterator it = first; it != last; ++it) {
    desc_dealoc_(it->second);
    ++count;
  }
  vehicles_by_time_.erase(first, last);
  return count;
}

template<class T, class TAlloc, class TDealloc>
std::vector<T> Checkpoint<T, TAlloc, TDealloc>::Get(
    const boost::posix_time::ptime& from,
    const boost::posix_time::ptime& to) const {
  typedef BOOST_TYPEOF(vehicles_by_time_.begin()) iterator;
  iterator first = vehicles_by_time_.lower_bound(from);
  iterator last = vehicles_by_time_.lower_bound(to);
  std::vector<T> result;
  for (iterator it = first; it != last; ++it) {
    result.push_back(it->second);
  }
  return result;
}

template<class T, class TAlloc, class TDealloc>
std::size_t Checkpoint<T, TAlloc, TDealloc>::Count(
    const boost::posix_time::ptime& from,
    const boost::posix_time::ptime& to) const {
  // TODO optimize
  return Get(from, to).size();
}

template<class T, class TAlloc, class TDealloc>
units::Frequency Checkpoint<T, TAlloc, TDealloc>::VehiclePassFrequency(
    const boost::posix_time::ptime& from,
    const boost::posix_time::ptime& to) const {
  std::size_t count = Count(from, to);
  boost::posix_time::time_duration duration = to - from;
  units::TimeDuration delta_t = duration.total_milliseconds() / 1000. *
      boost::units::si::second;
  return units::kOne * count / delta_t;
}

template<class T, class TAlloc, class TDealloc>
void Checkpoint<T, TAlloc, TDealloc>::
HandleVehicleDescriptor(const VehicleDescriptorEvent<T>& e) {
  Register(e.descriptor(), e.timestamp());
}

} // namespace slesann

#endif /* CHECKPOINT_H_ */
