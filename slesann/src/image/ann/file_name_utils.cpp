/*
 * file_name_utils.cpp
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
 * file_utils.cpp
 *
 *  Created on: May 12, 2011
 *      Author: losvald
 */

#include "../file_name_utils.h"
#include "file_name_utils.h"

namespace slesann {

namespace ann {

std::string GetLicenceNumber(const boost::filesystem::path& p) {
  return GetLicenceNumber(fs::basename(p));
}

std::string GetLicenceNumber(const std::string& file_basename) {
  std::string::size_type from = file_basename.find(kCheckpointSeparator);
  return file_basename.substr(0, from);
}

}  // namespace ann

}  // namespace slesann
