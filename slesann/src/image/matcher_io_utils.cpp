/*
 * matcher_io_utils.cpp
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
 * matcher_io_utils.cpp
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef MATCHER_IO_UTILS_CPP_
#define MATCHER_IO_UTILS_CPP_

#include <fstream>

#include "../file_utils.h"
#include "frame_info.h"
#include "matcher_io_utils.h"

namespace slesann {

std::vector<FrameInfo> ReadVehiclesToMatch(
    const boost::filesystem::path& input_file_path) {
  std::vector<FrameInfo> to_match;
  std::ifstream ifs;
  ifs.open(input_file_path.string().c_str());
  for (FrameInfo fi; ifs >> fi; ) {
    RequireExists(fi.image_path());
    RequireExists(fi.body_image_path());
    RequireExists(fi.plate_image_path());
    to_match.push_back(fi);
  }
  ifs.close();

  return to_match;
}

}  // namespace slesann

#endif /* MATCHER_IO_UTILS_CPP_ */
