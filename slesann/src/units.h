/*
 * units.h
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
 * units.h
 *
 *  Created on: Apr 5, 2011
 *      Author: losvald
 */

#ifndef UNITS_H_
#define UNITS_H_

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/frequency.hpp>
#include <boost/units/systems/si/dimensionless.hpp>

namespace slesann {

namespace units {

typedef double UnitRealType;

typedef boost::units::quantity<boost::units::si::length, UnitRealType> Distance;
typedef boost::units::quantity<boost::units::si::velocity, UnitRealType> Speed;
typedef boost::units::quantity<boost::units::si::frequency, UnitRealType> Frequency;
typedef boost::units::quantity<boost::units::si::time, UnitRealType> TimeDuration;

const boost::units::quantity<boost::units::si::dimensionless, UnitRealType> kOne = 1.0;

const Speed kmph = boost::units::si::metre_per_second / 3.6;

}  // namespace units

}  // namespace slesann





#endif /* UNITS_H_ */
