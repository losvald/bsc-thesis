/*
 * dummy_descriptor_provider.h
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
 * dummy_descriptor_provider.h
 *
 *  Created on: Apr 4, 2011
 *      Author: losvald
 */

#ifndef DUMMY_DESCRIPTOR_PROVIDER_H_
#define DUMMY_DESCRIPTOR_PROVIDER_H_

#include <iostream>
#include <ctime>
#include <queue>

#include "../dummy_base.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>

namespace slesann {

class DummyVehicleDescriptorProvider
: public VehicleDescriptorProvider<DummyVehicleDescriptor> {

private:

  std::queue<VehicleDescriptorEventType> events_;

public:

  bool GrabDescriptor() {
    if (events_.empty())
      return false;

    NotifyDescriptorGrabbed(events_.front());
    events_.pop();
    return true;
  }

  friend std::istream& operator>>(std::istream& is,
                                  DummyVehicleDescriptorProvider& vip) {
    std::time_t t;
    DummyVehicleDescriptor descriptor;
    while (is >> descriptor >> t) {
      VehicleDescriptorEventType e(descriptor, boost::posix_time::from_time_t(t));
      vip.events_.push(e);
    }
    return is;
  }

};

}  // namespace slesann

#endif /* DUMMY_DESCRIPTOR_PROVIDER_H_ */
