/*
 * vehicle_descriptor_handler.h
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
 * vehicle_descriptor_handler.h
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#ifndef VEHICLE_DESCRIPTOR_HANDLER_H_
#define VEHICLE_DESCRIPTOR_HANDLER_H_

#include "vehicle_descriptor_event.h"

#include <boost/shared_ptr.hpp>

namespace slesann {

template<class T>
class VehicleDescriptorHandler {
public:
  typedef boost::shared_ptr<VehicleDescriptorHandler> Pointer;

  /**
   * Handles the descriptor representing the vehicle.
   *
   * @param e event
   */
  virtual void HandleVehicleDescriptor(const VehicleDescriptorEvent<T>& e) = 0;
};

}  // namespace slesann

#endif /* VEHICLE_DESCRIPTOR_HANDLER_H_ */
