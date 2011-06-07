/*
 * color_hasher.h
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
 * color_hasher.h
 *
 *  Created on: May 14, 2011
 *      Author: losvald
 */

#ifndef COLOR_HASHER_H_
#define COLOR_HASHER_H_

#include "image_base.h"

#include <utility>

#include <boost/filesystem.hpp>
#include <opencv/cxcore.h>

namespace slesann {

typedef std::pair<uchar, uchar> UVPair;

cv::Vec3b ComputeColorHash(const cv::Mat& img);

cv::Vec3b ComputeColorHash(const ImagePath& img_path);

}  // namespace slesann

#endif /* COLOR_HASHER_H_ */
