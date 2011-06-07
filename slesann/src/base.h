/*
 * base.h
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
 * base.h
 *
 *  Created on: Mar 30, 2011
 *      Author: losvald
 */

#ifndef BASE_H_
#define BASE_H_

#include <iostream>
#include <cstdlib>

#define EZLOGGER_IMPLEMENT_DEBUGLOGGING // XXX move to Makefile / build config
//#define RUN_TESTS_ENABLED

// include assert if DEBUG LOGGING is on (required by some functions)
#ifdef EZLOGGER_IMPLEMENT_DEBUGLOGGING
#include <cassert>
#endif

#include "ezlogger/ezlogger_headers.hpp"

namespace slesann {

namespace environment {

static axter::verbosity verbosity = axter::log_default_verbosity_level; // verbosity level

void InitLogger();

} // namespace environment


namespace { // private functions

  void PrintTag(const std::string& tag) {
    if (!tag.empty())
        std::cerr << '[' << tag << ']' << ' ';
  }

} // end unnamed namespace

template<typename T> void PrintWarning(const T& to_print,
                                       const std::string& tag = "") {
  PrintTag(tag);
  std::cerr << "Warning: " << to_print << std::endl;
}

template<typename T> void PrintError(int return_code, const T& to_print,
                                     const std::string& tag = "") {
  PrintTag(tag);
  std::cerr << "Error: " << to_print << std::endl;
  if (environment::verbosity)
    std::cerr << "Exiting with return code " << return_code << std::endl;
  exit(return_code);
}

} // namespace slesann

#endif /* BASE_H_ */
