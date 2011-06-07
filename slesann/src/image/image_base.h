/*
 * image_base.h
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
 * image_base.h
 *
 *  Created on: May 14, 2011
 *      Author: losvald
 */

#ifndef IMAGE_BASE_H_
#define IMAGE_BASE_H_

#include <boost/filesystem/path.hpp>
#include <opencv/cxcore.h>

namespace slesann {

typedef boost::filesystem::path ImagePath;

const int kBgr = 0;
const int kYuv = 1;
const int kHsv = 2;

class EmptyColor {
public:
  template<int TColorSpace>
  cv::Vec3b asVec3b() const;
//  static const EmptyColor& instance();
  template<int TColorSpace>
  bool IsEmpty(const cv::Vec3b& color) const;
};

extern EmptyColor kEmptyColor;

template<int TColorSpace>
cv::Mat CropEmptyColorFrame(cv::Mat& img) {
  const int kLastCol = img.cols - 1;
  int x_min, x_max, y_min, y_max;
  for (int r = 0; r < img.rows; ++r) {
    int from = 0, to = kLastCol;
    while (from <= kLastCol &&
        kEmptyColor.IsEmpty<TColorSpace>(img.at<cv::Vec3b>(r, from)))
      ++from;
    while (to > from &&
        kEmptyColor.IsEmpty<TColorSpace>(img.at<cv::Vec3b>(r, to)))
      --to;
    if (from < to) {
      x_min = std::min(x_min, from);
      x_max = std::max(x_max, to);
      y_min = std::min(y_min, r);
      y_max = std::max(y_max, r);
    }
  }
  return cv::Mat(img, cv::Rect(x_min, y_min,
                               x_max - x_min + 1, y_max - y_min + 1));
}

std::string HueToString(uchar hue);

std::string SaturationToString(uchar saturation);

std::string IntensityToString(uchar intensity);

std::string ToBgrString(const cv::Vec3b& px);

std::string ToRgbString(const cv::Vec3b& px);

std::size_t GetPixelCount(const cv::Mat& img);

std::size_t GetPixelCount(const cv::Size& size);

}  // namespace slesann

#endif /* IMAGE_BASE_H_ */
