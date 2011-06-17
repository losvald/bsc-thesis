/*
 * image_base.cpp
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
 * image_base.cpp
 *
 *  Created on: May 14, 2011
 *      Author: losvald
 */

#include <cstdlib>

#include <algorithm>
#include <iomanip>
#include <iostream>

#include <opencv/cv.h>
#include <opencv/cxcore.h>

#include "../util/string_utils.h"
#include "image_base.h"

namespace slesann {

namespace {

int AbsDiff(uchar a, uchar b) {
  return abs((int)a - b);
}

}  // namespace

template<>
cv::Vec3b EmptyColor::asVec3b<kBgr>() const {
  return cv::Vec3b(0x00, 0xFF, 0x00);
}

template<>
cv::Vec3b EmptyColor::asVec3b<kYuv>() const {
  return cv::Vec3b(149, 43, 21);
}

template<>
cv::Vec3b EmptyColor::asVec3b<kHsv>() const {
  return cv::Vec3b(60, 0xFF, 0xFF);
}

template<>
bool EmptyColor::IsEmpty<kBgr>(const cv::Vec3b& color) const {
//  static const cv::Vec3b kThisColor = asVec3b<kBgr>();
//  return AbsDiff(kThisColor[0], color[0]) +
//      AbsDiff(kThisColor[1], color[1]) +
//      AbsDiff(kThisColor[2], color[2]) < 10;
//   return color[0] + color[2] < 8 && color[1] > 0xFF - 8; // for natural size
  return color[0] + color[2] < 39 && color[1] > 0xFF - 24;
}

template<>
bool EmptyColor::IsEmpty<kHsv>(const cv::Vec3b& color) const {
  static const cv::Vec3b kThisColor = asVec3b<kHsv>();
  return AbsDiff(kThisColor[0], color[0]) < 3 &&
      AbsDiff(kThisColor[1], color[1]) +
      AbsDiff(kThisColor[2], color[2]) < 10;
}

EmptyColor kEmptyColor;

cv::Vec3b BgrToHsv(const cv::Vec3b& bgr_color) {
  cv::Mat img_bgr(1, 1, CV_8UC3, cv::Scalar(bgr_color));
  cv::Mat img_hsv;
  cv::cvtColor(img_bgr, img_hsv, CV_BGR2HSV);
  return img_hsv.at<cv::Vec3b>(0, 0);
}

std::string HueToString(uchar hue) {
  return MakeString() << std::right << std::setw(3) <<
      2 * hue << " deg";
}

std::string SaturationToString(uchar saturation) {
  return MakeString() << std::fixed << std::setw(3) << std::setprecision(0) <<
      saturation / 2.55f << " %";
}

std::string IntensityToString(uchar intensity) {
  return MakeString() << std::fixed << std::setw(3) << std::setprecision(0) <<
      intensity / 2.55f << " %";
}

std::string ToBgrString(const cv::Vec3b& px) {
  return MakeString() << '(' << static_cast<int>(px[0]) << ", " <<
      static_cast<int>(px[1]) << ", " << static_cast<int>(px[2]) << ')';
}

std::string ToRgbString(const cv::Vec3b& px) {
  return ToBgrString(cv::Vec3b(px[2], px[1], px[0]));
}

std::string ToHsvString(const cv::Vec3b& px) {
  return MakeString() << '(' << HueToString(px[0]) << ", " <<
        SaturationToString(px[1]) << ", " << IntensityToString(px[2]) << ')';
}

std::size_t GetPixelCount(const cv::Mat& img) {
  return static_cast<std::size_t>(img.rows) *
      static_cast<std::size_t>(img.cols);
}

std::size_t GetPixelCount(const cv::Size& size) {
  return static_cast<std::size_t>(size.area());
}

}  // namespace slesann
