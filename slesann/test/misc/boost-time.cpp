/*
 * boost-time.cpp
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
 * boost-time.cpp
 *
 *  Created on: Apr 9, 2011
 *      Author: losvald
 */

#include "../test.h"

#ifndef SKIP_TESTS

#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#if 0
int main(int argc, char **argv) {
  namespace pt = boost::posix_time;

  pt::ptime cur_time = pt::microsec_clock::local_time();
  pt::ptime some_time = pt::from_time_t(23525);


  pt::time_duration t = pt::milliseconds(2);

  std::cout << "Simple string: " << pt::to_simple_string(some_time) << std::endl;
  std::cout << t << std::endl;

  return 0;
}
#endif

#endif
