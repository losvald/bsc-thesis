/*
 * dummy_processing_loop_factory.h
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
 * dummy_processing_loop_factory.h
 *
 *  Created on: Apr 9, 2011
 *      Author: losvald
 */

#ifndef DUMMY_PROCESSING_LOOP_FACTORY_H_
#define DUMMY_PROCESSING_LOOP_FACTORY_H_

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>

#include "../base.h"
#include "../speed_limit_detector.h"
#include "../units.h"
#include "../vehicle_descriptor_provider.h"
#include "dummy_base.h"
#include "dummy_checkpoint.h"
#include "dummy_descriptor_provider.h"
#include "dummy_descriptor_matcher.h"

namespace slesann {

namespace factory {

VehicleDescriptorProvider<DummyVehicleDescriptor>::SharedPtr
InputDummyDescriptorProvider(const std::string& in_filename) {
  DummyVehicleDescriptorProvider::SharedPtr vdp(
      new DummyVehicleDescriptorProvider);

  EZDBGONLYLOGGERSTREAM2(std::cerr) << "Reading dummy provider"
        " from file: " << in_filename << std::endl;

  std::ifstream in;
  in.open(in_filename.c_str());
  BOOST_AUTO(dummy_vdp, boost::shared_static_cast<DummyVehicleDescriptorProvider>(vdp));
  in >> *dummy_vdp;
  in.close();

  EZLOGGERSTREAM2(std::cerr) << "Read dummy provider" << *dummy_vdp << std::endl;
  return vdp;
}

namespace {

SpeedLimitDetector<DummyVehicleDescriptor>::SharedPtr
InputSpeedLimitDetector(const std::string& limits_filename) {
//  units::Distance distance = 100. * boost::units::si::meter;
//  units::Speed min_speed = 10. * boost::units::si::metres_per_second;
//  units::Speed max_speed = 100. * boost::units::si::metres_per_second;

  EZDBGONLYLOGGERSTREAM2(std::cerr) << "Reading distance and speed limits" <<
      " from file: " << limits_filename << std::endl;

  std::ifstream in;
  in.open(limits_filename.c_str());
  units::UnitRealType min_speed_val, max_speed_val, distance_val;
  in >> distance_val >> min_speed_val >> max_speed_val;
  in.close();

  units::Distance distance = distance_val * boost::units::si::meter;
  units::Speed min_speed = min_speed_val * boost::units::si::metres_per_second;
  units::Speed max_speed = max_speed_val * boost::units::si::metres_per_second;

  EZDBGONLYLOGGERSTREAM2(std::cerr) <<
      "Distance: " << distance_val << std::endl <<
      "Min speed: " << min_speed_val << std::endl <<
      "Max speed: " << max_speed_val << std::endl;

  return SpeedLimitDetector<DummyVehicleDescriptor>::SharedPtr(
      new SpeedLimitDetector<DummyVehicleDescriptor>(
          0, 1,
          distance,
          min_speed,
          max_speed,
          boost::shared_ptr<DummyDescriptorMatcher>(
              new DummyDescriptorMatcher)
      )
  );
}

}  // namespace

ProcessingLoop<DummyVehicleDescriptor>::SharedPtr CreateDummyProcessingLoop(
    const std::string& start_cp_filename,
    const std::string& end_cp_filename,
    const std::string& limits_filename) {
  return ProcessingLoop<DummyVehicleDescriptor>::SharedPtr(
      new ProcessingLoop<DummyVehicleDescriptor>(
          InputSpeedLimitDetector(limits_filename),
          InputDummyDescriptorProvider(start_cp_filename),
          InputDummyDescriptorProvider(end_cp_filename)
      )
  );
}

}  // namespace factory

}  // namespace slesann

#endif /* DUMMY_PROCESSING_LOOP_FACTORY_H_ */
