/*
 * frame_info.h
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
 * frame_info.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef FRAME_INFO_H_
#define FRAME_INFO_H_

#include <iostream>

#include "image_base.h"

#include <boost/date_time/posix_time/ptime.hpp>

namespace slesann {

class FrameInfo {
  boost::posix_time::ptime timestamp_;
  ImagePath image_path_;
  ImagePath body_path_;
  ImagePath plate_path_;
public:
  FrameInfo();
  FrameInfo(const boost::posix_time::ptime& timestamp,
            const ImagePath& image_path);

  friend std::ostream& operator<<(std::ostream& os, const FrameInfo& f);
  friend std::istream& operator>>(std::istream& os, FrameInfo& f);

  const ImagePath& image_path() const;
  const ImagePath& body_image_path() const;
  const ImagePath& plate_image_path() const;
  void set_image_path(const ImagePath& path);
  void set_body_image_path(const ImagePath& path);
  void set_plate_image_path(const ImagePath& path);
};

}  // namespace slesann

#endif /* FRAME_INFO_H_ */
