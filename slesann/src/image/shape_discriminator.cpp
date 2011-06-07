/*
 * shape_discriminator.cpp
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
 * shape_discriminator.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include <algorithm>
#include <vector>
#include <set>

#include <opencv/highgui.h>
#include <boost/foreach.hpp>

#include "shape_discriminator.h"
#include "image_loader.h"
#include "transform_utils.h"
#include "ann/image_feature.h"

namespace slesann {

namespace {

int ComputeMedian(std::vector<int>* v) {
  std::size_t mid = v->size() / 2;
  std::nth_element(v->begin(), v->begin() + mid, v->end());
  return (*v)[mid];
}

void FilterHullPointsBySide(const Hull& h, std::vector<int>* left,
                            std::vector<int>* right) {
  int mid_x = h.bounding_box().center().x();
  std::size_t n = h.pixel_positions().size();
  left->clear(); left->reserve(n);
  right->clear(); right->reserve(n);
  BOOST_FOREACH(const PixelPosition& pp, h.pixel_positions()) {
    if (pp.x() <= mid_x)
      left->push_back(pp.x());
    if (pp.x() >= mid_x)
      right->push_back(pp.x());
  }
}

}  // namespace

const float ShapeDiscriminator::kResizeFactor = 0.2;

float ShapeDiscriminator::ComputeSimilarity(const ImagePath& img_path1,
                                            const ImagePath& img_path2) const {
  cv::Mat img1 = LoadImage(img_path1);
  std::cerr << "Shape" << std::endl;
}

float ShapeDiscriminator::ComputeUniqueness(const ImagePath& img_path,
                        const std::vector<float>& match_candidates) const {
  return *std::max_element(match_candidates.begin(),
                          match_candidates.end()) < 0.5f ? 0.f : 1.f;
}

cv::Mat ShapeDiscriminator::LoadImage(const ImagePath& img_path) {
  cv::Mat img_orig_size = ImageLoader::instance().LoadImage(img_path);
  return Resize(img_orig_size, kResizeFactor);
}

float ShapeDiscriminator::ComputeWidthHeightRatio(const Hull& h) {
  return (float)h.bounding_box().width() / h.bounding_box().height();
}

float ShapeDiscriminator::GetNormalizedHeight(const Hull& h) {
  const Rectangle& bounding_box = h.bounding_box();
  return static_cast<float>(2.f * bounding_box.height()) /
      bounding_box.perimeter();
}

float ShapeDiscriminator::GetHeightPerimeterQuotient(const Hull& h) {
  return static_cast<float>(h.bounding_box().height()) / h.perimeter();
}

float ShapeDiscriminator::GetWidthPerimeterQuotient(const Hull& h) {
  return static_cast<float>(h.bounding_box().width()) / h.perimeter();
}

float ShapeDiscriminator::GetPerimeterQuotient(const Hull& h) {
  float c = h.perimeter();
  return 4 * 3.1415926536 * h.convex_area() / (c * c);
}

float ShapeDiscriminator::GetRectangularity(const Hull& h) {
  return (float)h.convex_area() / h.bounding_box().area();
}

float ShapeDiscriminator::GetSideFunctionMedian(const Hull& h) {
  std::vector<int> left_xs, right_xs;
  FilterHullPointsBySide(h, &left_xs, &right_xs);
  const Rectangle& bb = h.bounding_box();
  int mid_x = bb.center().x();

  float left_ratio = 2.f * (mid_x - ComputeMedian(&left_xs)) / bb.width();
  float right_ratio = 2.f * (ComputeMedian(&right_xs) - mid_x) / bb.width();
  return (fabs(left_ratio) + fabs(right_ratio)) / 2;
}

float ShapeDiscriminator::GetSideFunctionAverage(const Hull& h) {
  const Rectangle& bb = h.bounding_box();
  int mid_x = bb.center().x();
  std::size_t left_count = 0, right_count = 0;
  int left_sum = 0, right_sum = 0;
  BOOST_FOREACH(const PixelPosition& pp, h.pixel_positions()) {
    if (pp.x() <= mid_x) {
      left_sum += pp.x();
      ++left_count;
    }
    if (pp.x() >= mid_x) {
      right_sum += pp.x();
      ++right_count;
    }
  }
  float left_avg = static_cast<float>(left_sum) / left_count;
  float right_avg = static_cast<float>(right_sum) / right_count;

  float left_ratio = 2.f * (mid_x - left_avg) / bb.width();
  float right_ratio = 2.f * (right_avg - mid_x) / bb.width();
  return (fabs(left_ratio) + fabs(right_ratio)) / 2;
}

}  // namespace slesann
