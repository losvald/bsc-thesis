/*
 * vehicle_descriptor_event.h
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
 * vehicle_descriptor_event.h
 *
 *  Created on: Apr 4, 2011
 *      Author: losvald
 */

#ifndef VEHICLE_DESCRIPTOR_EVENT_H_
#define VEHICLE_DESCRIPTOR_EVENT_H_

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace slesann {

template<class T>
class VehicleDescriptorEvent {
public:
  VehicleDescriptorEvent(const T& descriptor, const boost::posix_time::ptime& timestamp);
  VehicleDescriptorEvent(const T& descriptor);
  const T& descriptor() const;
  const boost::posix_time::ptime& timestamp() const;
private:
  void init();

  const T descriptor_;
  const boost::posix_time::ptime& timestamp_;
};

template<class T>
VehicleDescriptorEvent<T>::VehicleDescriptorEvent(
    const T& descriptor,
    const boost::posix_time::ptime& timestamp)
    : descriptor_(descriptor),
      timestamp_(timestamp) {

}

template<class T>
VehicleDescriptorEvent<T>::VehicleDescriptorEvent(const T& descriptor)
: descriptor_(descriptor),
  timestamp_(boost::posix_time::microsec_clock::local_time()) {

}

template<class T>
const T& VehicleDescriptorEvent<T>::descriptor() const {
  return descriptor_;
}

template<class T>
const boost::posix_time::ptime& VehicleDescriptorEvent<T>::timestamp() const {
  return timestamp_;
}

} // namespace slesann

#endif /* VEHICLE_DESCRIPTOR_EVENT_H_ */
