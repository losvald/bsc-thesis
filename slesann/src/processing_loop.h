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

#include "speed_limit_detector.h"
#include "descriptor_memory.h"

#include "boost/shared_ptr.hpp"

namespace slesann {

template<class T, class TAlloc = DefaultDescriptorAllocator<T>,
    class TDealloc = DefaultDescriptorDeallocator<T> >
class ProcessingLoop {

public:
  typedef boost::shared_ptr< ProcessingLoop<T, TAlloc, TDealloc> > SharedPtr;
  typedef VehicleDescriptorProvider<T> VehicleDescriptorProviderType;
  typedef SpeedLimitDetector<T, TAlloc, TDealloc> SpeedLimitDetectorType;
  typedef typename SpeedLimitDetectorType::SharedPtr
      SpeedLimitDetectorSharedPtr;
  typedef typename VehicleDescriptorProvider<T>::SharedPtr
      VehicleDescriptorProviderSharedPtr;

  ProcessingLoop(SpeedLimitDetectorSharedPtr speed_limit_detector,
                 VehicleDescriptorProviderSharedPtr desc_provider_start,
                 VehicleDescriptorProviderSharedPtr desc_provider_end);

  virtual void run();

protected:
  void init();

private:

  SpeedLimitDetectorSharedPtr speed_limit_detector_;
  VehicleDescriptorProviderSharedPtr desc_provider_start_;
  VehicleDescriptorProviderSharedPtr desc_provider_end_;
};

template<class T, class TAlloc, class TDealloc>
ProcessingLoop<T, TAlloc, TDealloc>::ProcessingLoop(
    SpeedLimitDetectorSharedPtr speed_limit_detector,
    VehicleDescriptorProviderSharedPtr desc_provider_start,
    VehicleDescriptorProviderSharedPtr desc_provider_end)
    : speed_limit_detector_(speed_limit_detector),
      desc_provider_start_(desc_provider_start),
      desc_provider_end_(desc_provider_end) {
  init();
}

template<class T, class TAlloc, class TDealloc>
void ProcessingLoop<T, TAlloc, TDealloc>::run() {
  namespace pt = boost::posix_time;

  while (desc_provider_start_->GrabDescriptor() ||
      desc_provider_end_->GrabDescriptor()) {
  }

  speed_limit_detector_->DetectViolators(pt::ptime(pt::pos_infin));
}

template<class T, class TAlloc, class TDealloc>
void ProcessingLoop<T, TAlloc, TDealloc>::init() {
  typedef typename VehicleDescriptorProviderType::VehicleDescriptorHandlerType
      HandlerType;
  typedef typename VehicleDescriptorProviderType::VehicleDescriptorHandlerSharedPtr
      HandlerSharedPtr;

  HandlerSharedPtr start_handler(&speed_limit_detector_->start_checkpoint());
  desc_provider_start_->AddHandler(start_handler);
  HandlerSharedPtr end_handler(&speed_limit_detector_->end_checkpoint());
   desc_provider_end_->AddHandler(end_handler);
}

}  // namespace slesann

#endif /* PROCESSING_LOOP_H_ */
