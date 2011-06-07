/*
 * binary_image_utils.h
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
 * binary_image_utils.h
 *
 *  Created on: Jun 3, 2011
 *      Author: losvald
 */

#ifndef BINARY_IMAGE_UTILS_H_
#define BINARY_IMAGE_UTILS_H_

#include <cassert>

#include "image_base.h"

namespace slesann {

namespace {

inline std::size_t ComputeDelta(int img_dim, std::size_t n) {
  std::size_t lo = 0, hi = img_dim;
  while (lo < hi) {
    std::size_t mid = (lo + hi + 1) >> 1;
    if (mid * (n - 1) < static_cast<std::size_t>(img_dim))
      lo = mid;
    else
      hi = mid - 1;
  }
  return lo;
}

}  // namespace

class ThinRegionRemover {
//  enum ImageSize { kSmallImage, kMediumImage, kLargeImage };
protected:
  typedef std::pair<int, double> AdaptiveThresholdParams;
  typedef std::map<int, double> AdaptiveThresholdParamMap;

  static cv::Mat AdaptiveThreshold(const cv::Mat& img_binary,
                                   const AdaptiveThresholdParams& params);

  virtual AdaptiveThresholdParamMap CreateMap(const cv::Mat& img_binary,
                                              std::size_t itr) const = 0;

  virtual int DetermineMaxBlockSize(const cv::Mat& img_binary,
                                    std::size_t itr) const = 0;

public:

  virtual ~ThinRegionRemover();

  cv::Mat operator()(const cv::Mat& img_binary, std::size_t itr) const;

};

cv::Mat RemoveThinRegions(const cv::Mat& img_binary,
                          const ThinRegionRemover& thin_region_remover,
                          std::size_t iterations);

template<typename T, typename U, class MapFunc>
void GetPixelValueSumsAlongX(const cv::Mat& img, std::size_t n,
                            const MapFunc& mapper,
                            std::vector<U>* y_sums) {
  if (static_cast<int>(n) > img.cols)
    n = img.cols;
  *y_sums = std::vector<U>(n);
  std::size_t dx = ComputeDelta(img.cols, n);
  int last_chunk_x = (n - 1) * dx;
  for (int y = 0; y < img.rows; ++y) {
    std::size_t i = 0, i_div_n = 0;
    for (int x = 0; x < last_chunk_x; ++x) {
      (*y_sums)[i_div_n] += mapper(img.at<T>(y, x));
      if (++i == dx) {
        i -= dx;
        ++i_div_n;
      }
    }
    for (int x = last_chunk_x; x < img.cols; ++x)
      (*y_sums)[i_div_n] += mapper(img.at<T>(y, x));
  }
}

template<typename T, typename U, class MapFunc>
void GetPixelValueSumsAlongY(const cv::Mat& img, std::size_t n,
                            const MapFunc& mapper,
                            std::vector<U>* x_sums) {
  if (static_cast<int>(n) > img.rows)
    n = img.rows;
  *x_sums = std::vector<U>(n);
  std::size_t dy = ComputeDelta(img.rows, n);
  int last_chunk_y = (n - 1) * dy;
  std::size_t i = 0, i_div_n = 0;
  for (int y = 0; y < last_chunk_y; ++y) {
    for (int x = 0; x < img.cols; ++x)
      (*x_sums)[i_div_n] += mapper(img.at<T>(y, x));
    if (++i == dy) {
      i -= dy;
      ++i_div_n;
    }
  }
  for (int y = last_chunk_y; y < img.rows; ++y)
    for (int x = 0; x < img.cols; ++x)
      (*x_sums)[i_div_n] += mapper(img.at<T>(y, x));
}

template<typename T, typename U,
class MapFunc, class InverseMapFunc, class DivisionFunc>
void GetPixelValueAveragesAlongX(const cv::Mat& img, std::size_t n,
                                 const MapFunc& mapper,
                                 const InverseMapFunc& inverse_mapper,
                                 const DivisionFunc& divider,
                                 std::vector<T>* y_avgs) {
  std::vector<U> y_sums;
  GetPixelValueSumsAlongX<uchar, U>(img, n, mapper, &y_sums);
  n = y_sums.size();
  *y_avgs = std::vector<T>(n);
  assert(y_sums.size() == n);

  const std::size_t dx = ComputeDelta(img.cols, n);
  const std::size_t dx_area = dx * img.rows, tot_area = img.rows * img.cols;
  std::size_t dx_area_acum = 0;
  for (std::size_t i = 0; i < n - 1; ++i) {
    (*y_avgs)[i] = divider(y_sums[i], dx_area);
    dx_area_acum += dx_area;
  }
  y_avgs->back() = divider(y_sums.back(), tot_area - dx_area_acum);
}

template<typename T, typename U,
class MapFunc, class InverseMapFunc, class DivisionFunc>
void GetPixelValueAveragesAlongY(const cv::Mat& img, std::size_t n,
                                 const MapFunc& mapper,
                                 const InverseMapFunc& inverse_mapper,
                                 const DivisionFunc& divider,
                                 std::vector<T>* x_avgs) {
  std::vector<U> x_sums;
  GetPixelValueSumsAlongY<uchar, U>(img, n, mapper, &x_sums);
  n = x_sums.size();
  *x_avgs = std::vector<T>(n);
  assert(x_sums.size() == n);

  const std::size_t dy = ComputeDelta(img.rows, n);
  const std::size_t dy_area = dy * img.cols, tot_area = img.rows * img.cols;
  std::size_t dy_area_acum = 0;
  for (std::size_t i = 0; i < n - 1; ++i) {
    (*x_avgs)[i] = divider(x_sums[i], dy_area);
    dy_area_acum += dy_area;
  }
  x_avgs->back() = divider(x_sums.back(), tot_area - dy_area_acum);
}

std::size_t GetBlackPixelCount(const cv::Mat& img_binary, int y_from, int y_to);

void GetVerticalBlackWhiteRatios(const cv::Mat& img_binary,
                                 std::size_t n,
                                 std::vector<float>* norm_y_avgs);

void GetHorizontalBlackWhiteRatios(const cv::Mat& img_binary,
                                   std::size_t n,
                                   std::vector<float>* norm_x_avgs);

}  // namespace slesann

#endif /* BINARY_IMAGE_UTILS_H_ */
