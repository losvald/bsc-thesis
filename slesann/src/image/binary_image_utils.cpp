/*
 * binary_image_utils.cpp
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
 * binary_image_utils.cpp
 *
 *  Created on: Jun 3, 2011
 *      Author: losvald
 */

#include <boost/foreach.hpp>
#include <opencv/cv.h>

#include "binary_image_utils.h"

namespace slesann {

namespace {



}  // namespace

ThinRegionRemover::~ThinRegionRemover() {}

cv::Mat ThinRegionRemover::AdaptiveThreshold(
    const cv::Mat& img_binary,
    const AdaptiveThresholdParams& params) {
  cv::Mat img_thres;
  cv::adaptiveThreshold(img_binary, img_thres, 0xFF,
                        CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,
                        params.first, params.second);
  return img_thres;
}

cv::Mat ThinRegionRemover::operator()(const cv::Mat& img_binary,
                                      std::size_t itr) const {
  int max_block_size = DetermineMaxBlockSize(img_binary, itr);
  AdaptiveThresholdParamMap thr_map = CreateMap(img_binary, itr);
  cv::Mat min(img_binary.rows, img_binary.cols, CV_8U, cv::Scalar(0xFF));
  BOOST_FOREACH(const AdaptiveThresholdParams& p, thr_map) {
    int block_size = p.first;
    if (block_size > max_block_size)
      break;

//    double c = p.second;
//    std::cerr << "Applying adaptive threshold (iteration = " << itr <<
//        "): block size = " << block_size << "; c = " << c << std::endl;

    cv::Mat img_thres = AdaptiveThreshold(img_binary, p);
    cv::min(img_thres, min, min);

//    cv::imshow("min", min);
//    cv::waitKey(-1);
  }
  return min;
}


cv::Mat RemoveThinRegions(const cv::Mat& img_binary,
                          const ThinRegionRemover& thin_region_remover,
                          std::size_t iterations) {
  const cv::Mat all_white(img_binary.rows, img_binary.cols, CV_8U,
                          cv::Scalar(0xFF));
  cv::Mat cur_img = img_binary.clone();
  for (std::size_t itr = 0; itr < iterations; ++itr) {
    cv::Mat img_filtered = thin_region_remover(cur_img, itr);
    cv::Mat img_filtered_compl;
    cv::subtract(all_white, img_filtered, img_filtered_compl);
    cv::max(img_filtered_compl, cur_img, cur_img);
  }
  return cur_img;
}

std::size_t GetBlackPixelCount(const cv::Mat& img_binary, int y_from,
                               int y_to) {
  std::size_t count = 0;
  for (int y = y_from; y < y_to; ++y)
    for (int x = 0; x < img_binary.cols; ++x)
      count += !img_binary.at<uchar>(y, x);
  return count;
}

template<typename T>
class UcharToPrimitiveMapper {
public:
  T operator()(uchar pixel_value) const {
    return static_cast<T>(pixel_value);
  }
};

template<typename T>
class PrimitiveToUcharMapper {
public:
  T operator()(const T& pixel_value) const {
    return cv::saturate_cast(pixel_value);
  }
};

template<typename T>
class PrimitiveDivider {
public:
  uchar operator()(const T& pixel_value, int divisor) const {
    return cv::saturate_cast<uchar>(pixel_value / divisor);
  }
};

void GetVerticalBlackWhiteRatios(const cv::Mat& img_binary,
                                 std::size_t n,
                                 std::vector<float>* norm_y_avgs) {
  std::vector<uchar> y_avgs;
  GetPixelValueAveragesAlongX<uchar, int>(img_binary, n,
                                          UcharToPrimitiveMapper<float>(),
                                          PrimitiveToUcharMapper<float>(),
                                          PrimitiveDivider<float>(),
                                          &y_avgs);
  n = y_avgs.size();
  norm_y_avgs->resize(n);
  for (std::size_t i = 0; i < n; ++i)
    (*norm_y_avgs)[i] = 1.f - y_avgs[i] / 255.f;
}

void GetHorizontalBlackWhiteRatios(const cv::Mat& img_binary,
                                   std::size_t n,
                                   std::vector<float>* norm_x_avgs) {
  std::vector<uchar> x_avgs;
  GetPixelValueAveragesAlongY<uchar, int>(img_binary, n,
                                          UcharToPrimitiveMapper<float>(),
                                          PrimitiveToUcharMapper<float>(),
                                          PrimitiveDivider<float>(),
                                          &x_avgs);
  n = x_avgs.size();
  norm_x_avgs->resize(n);
  for (std::size_t i = 0; i < n; ++i)
    (*norm_x_avgs)[i] = 1.f - x_avgs[i] / 255.f;
}


}  // namespace slesann
