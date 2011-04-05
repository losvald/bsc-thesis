/*
 * vehicle_descriptor_provider.h
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
 * vehicle_descriptor_provider.h
 *
 *  Created on: Apr 4, 2011
 *      Author: losvald
 */

#ifndef VEHICLE_DESCRIPTOR_PROVIDER_H_
#define VEHICLE_DESCRIPTOR_PROVIDER_H_

#include <vector>

#include "vehicle_descriptor_event.h"
#include "vehicle_descriptor_handler.h"

#include <boost/foreach.hpp>

namespace slesann {

template<class T>
class VehicleDescriptorProvider {
public:
  typedef VehicleDescriptorHandler<T> VehicleDescriptorHandlerType;
  typedef typename VehicleDescriptorHandlerType::Pointer VehicleDescriptorHandlerPointerType;
  typedef VehicleDescriptorEvent<T> VehicleDescriptorEventType;

  void AddHandler(VehicleDescriptorHandlerPointerType handler);

  /**
   * Grabs the next descriptor. The appropriate event will be fired,
   * triggering the registered handlers.
   *
   * @return true if any descriptor has been grabbed
   */
  virtual bool GrabDescriptor() = 0;

protected:
  void NotifyDescriptorGrabbed(const VehicleDescriptorEventType& e);

private:
  std::vector<VehicleDescriptorHandlerPointerType> handlers_;
};


template<class T>
void VehicleDescriptorProvider<T>::AddHandler(
    VehicleDescriptorHandlerPointerType handler) {
  handlers_.push_back(handler);
}

template<class T>
void VehicleDescriptorProvider<T>::NotifyDescriptorGrabbed(const VehicleDescriptorEventType& e) {
  BOOST_FOREACH(typename VehicleDescriptorHandler<T>::Pointer h, handlers_) {
    h->HandleVehicleDescriptor(e);
  }
}

}  // namespace slesann

#endif /* VEHICLE_DESCRIPTOR_PROVIDER_H_ */
