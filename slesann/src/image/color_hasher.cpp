/*
 * color_hasher.cpp
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
 * color_hasher.cpp
 *
 *  Created on: May 14, 2011
 *      Author: losvald
 */

#include <algorithm>
#include <map>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "../base.h"
#include "color_hasher.h"
#include "image_base.h"

namespace slesann {

namespace {

UVPair ToUV(const cv::Vec3b& yuv) {
  return UVPair(yuv[1], yuv[2]);
}

}  // namespace

cv::Vec3b ComputeColorHash(const cv::Mat& img) {
  cv::Mat img_yuv;
  cv::cvtColor(img, img_yuv, CV_BGR2YCrCb);

  cv::Vec3b median;

  median = kEmptyColor.asVec3b<kYuv>();

  cv::Vec2b b, a;
  std::map<UVPair, std::size_t> uv_count;

  for (int r = 0; r < img.rows; ++r) {
    for (int c = 0; c < img.cols; ++c) {
      ++uv_count[ToUV(img.at<cv::Vec3b>(r, c))];
    }
  }

  std::cerr << "UV by frequencies: " << std::endl;
  typedef std::map<UVPair, std::size_t>::const_iterator Iter;
  int itr = 0;
  for (Iter it = uv_count.begin(), end = uv_count.end(); it != end; ++it) {
    std::cerr << "(" << it->first.first << ", " << it->first.second << ") = " <<
        it->second << std::endl;
    if (++itr > 20)
      break;
  }

  return median;
}

cv::Vec3b ComputeColorHash(const boost::filesystem::path& img_path) {
  return ComputeColorHash(cv::imread(img_path.string()));
}

}  // namespace slesann
