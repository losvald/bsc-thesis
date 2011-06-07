/*
 * median_color_discriminator.cpp
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
 * median_color_discriminator.cpp
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#include <algorithm>

#include <opencv/cv.h>
#include <opencv/highgui.h> // XXX debug
#include <boost/filesystem/path.hpp>

#include "median_color_discriminator.h"
#include "transform_utils.h"

namespace slesann {

namespace {

typedef std::pair<uchar, uchar> HVPair;

HVPair ToHV(const cv::Vec3b& hsv) {
  return HVPair(hsv[0], hsv[2]);
}

HVPair ComputeMedianHV(const cv::Mat& img, const std::string& win_name = "?") {
  cv::Mat img_scaled_down, img_hsv;
  img_scaled_down = Resize(img, 0.25f);
  cv::cvtColor(img_scaled_down, img_hsv, CV_BGR2HSV);

  HVPair median;
  median.second = 0;
  std::vector<uchar> hue;

  cv::Vec2b b, a;
  std::map<HVPair, std::size_t> uv_count;

//  const cv::Vec3b& px55 = img_scaled_down.at<cv::Vec3b>(5, 5);
//  std::cerr << "[" << (int)px55[0] << ", " << (int)px55[1] << ", " <<
//            (int)px55[2] << "]\n";

  for (int r = 0; r < img_scaled_down.rows; ++r) {
    for (int c = 0; c < img_scaled_down.cols; ++c) {
      const cv::Vec3b& px = img_scaled_down.at<cv::Vec3b>(r, c);
//      std::cerr << "[" << (int)px[0] << ", " << (int)px[1] << ", " <<
//          (int)px[2] << "]\n";
      ++uv_count[ToHV(px)];
      hue.push_back(px[0]);
      median.second += px[2];
    }
  }

  std::cerr << "HV by frequencies: " << std::endl;
  typedef std::map<HVPair, std::size_t>::const_iterator Iter;
  int itr = 0;
  for (Iter it = uv_count.begin(), end = uv_count.end(); it != end; ++it) {
    std::cerr << "(" << (int)it->first.first << ", " <<
        (int)it->first.second << ") = " <<
        it->second << std::endl;
    if (++itr > 20)
      break;
  }

  int mid = hue.size() / 2;
  nth_element(hue.begin(), hue.begin() + mid, hue.end());
  median.first = hue[mid];
  median.second /= hue.size();

  std::cerr << "Median = (" << (int)median.first << ", " <<
      (int)median.second << ") = " << std::endl;
  cv::imshow(win_name, img_scaled_down);
  cv::waitKey(-1);

  return median;
}

HVPair ComputeMedianHV(const ImagePath& img_path) {
  return ComputeMedianHV(cv::imread(img_path.string()), img_path.string());
}

} // namespace

float MedianColorDiscriminator::ComputeSimilarity(const ImagePath& d1,
                                                  const ImagePath& d2) const {
  HVPair median2 = ComputeMedianHV(d2);
  HVPair median1 = ComputeMedianHV(d1);

  // TODO

  return 0;
}

float MedianColorDiscriminator::ComputeUniqueness(
    const ImagePath& d,const std::vector<float>& match_candidates) const {
  return *std::max_element(match_candidates.begin(),
                          match_candidates.end()) < 0.5f ? 0.f : 1.f;
}


}  // namespace slesann
