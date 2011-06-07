/*
 * frame_info.cpp
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
 * frame_info.cpp
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#include "frame_info.h"

#include "../util/time_utils.h"

namespace slesann {

FrameInfo::FrameInfo() {}

FrameInfo::FrameInfo(const boost::posix_time::ptime& timestamp,
                     const ImagePath& image_path)
: timestamp_(timestamp), image_path_(image_path) {}

std::ostream& operator<<(std::ostream& os, const FrameInfo& f) {
  os << "FrameInfo[timestamp = " << PtimeToMillisecStrFromEpoch(f.timestamp_) <<
      "; image_path = " << f.image_path_.string() <<
      "; body_path = " << f.body_path_.string() <<
      "; plate_path = " << f.plate_path_.string() << ']';
  return os;
}

std::istream& operator>>(std::istream& is, FrameInfo& f) {
  std::string timestamp_str, image_path_str, body_path_str, plate_path_str;
  is >> timestamp_str;
  is >> image_path_str;
  is >> body_path_str;
  is >> plate_path_str;
  f.timestamp_ = MillisecStrFromEpochToPtime(timestamp_str);
  f.image_path_ = ImagePath(image_path_str);
  f.body_path_ = ImagePath(body_path_str);
  f.plate_path_ = ImagePath(plate_path_str);
  return is;
}

const ImagePath& FrameInfo::image_path() const {
  return image_path_;
}

const ImagePath& FrameInfo::body_image_path() const {
  return body_path_;
}

const ImagePath& FrameInfo::plate_image_path() const {
  return plate_path_;
}

void FrameInfo::set_image_path(const ImagePath& path) {
  image_path_ = path;
}

void FrameInfo::set_body_image_path(const ImagePath& path) {
  body_path_ = path;
}

void FrameInfo::set_plate_image_path(const ImagePath& path) {
  plate_path_ = path;
}

}  // namespace slesann
