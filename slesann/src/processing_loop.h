/*
 * processing_loop.h
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
 * processing_loop.h
 *
 *  Created on: Apr 4, 2011
 *      Author: losvald
 */

#ifndef PROCESSING_LOOP_H_
#define PROCESSING_LOOP_H_

//#include <boost/shared_ptr.hpp>

#include "vehicle_descriptor_handler.h"
#include "vehicle_descriptor_provider.h"

#include "matching/matcher.h"
#include "descriptor_memory.h"

namespace slesann {

template<class T, class TAlloc = DefaultDescriptorAllocator<T>,
    class TDealloc = DefaultDescriptorDeallocator<T> >
class ProcessingLoop {

public:
  typedef VehicleDescriptorProvider<T> VehicleDescriptorProviderType;
  typedef SpeedLimitEnforcer<T, TAlloc, TDealloc> SpeedLimitEnforcerType;

  ProcessingLoop(SpeedLimitEnforcerType* speed_limit_enforcer);

private:

  SpeedLimitEnforcerType* speed_limit_enforcer_;
};

template<class T, class TAlloc, class TDealloc>
ProcessingLoop<T, TAlloc, TDealloc>::ProcessingLoop(
    SpeedLimitEnforcerType* speed_limit_enforcer)
    : speed_limit_enforcer_(speed_limit_enforcer) {
}

}  // namespace slesann

#endif /* PROCESSING_LOOP_H_ */
