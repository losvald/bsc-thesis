/*
 * transform_utils.cpp
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
 * transform_utils.cpp
 *
 *  Created on: May 14, 2011
 *      Author: losvald
 */

#include "transform_utils.h"

#include "opencv/cv.h"

namespace slesann {

cv::Mat Resize(const cv::Mat& img, float factor) {
  cv::Mat ret;
  cv::Size size(img.cols * factor, img.rows * factor);
  cv::resize(img, ret, size);
  return ret;
}

}  // namespace slesann
