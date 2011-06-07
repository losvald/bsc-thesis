/*
 * image_feature.cpp
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
 * image_feature.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#include "../image_base.h"
#include "image_feature.h"

#include <opencv/cv.h>

namespace slesann {

namespace ann {

ImageFeature::ImageFeature(const std::string& name,
                           std::size_t input_vector_size)
: name_(name), input_vector_size_(input_vector_size) {}

InputVector<float> ImageFeature::operator()() const {
  if (input_vector_cache_.empty()) {
    input_vector_cache_.resize(input_vector_size_);
    Compute(&input_vector_cache_);
  }
  return input_vector_cache_;
}

std::ostream& operator<<(std::ostream& os, const ImageFeature& f) {
  os << f.name_ << " = " << f();
  return os;
}

const std::string& ImageFeature::name() const {
  return name_;
}

void ImageFeature::ClearCache() const {
  input_vector_cache_.clear();
}



OneValueImageFeature::OneValueImageFeature(const std::string& name,
                                           float value)
: super(name, 1), value_(value) {}

float OneValueImageFeature::value() const {
  return value_;
}


NormalizedValueImageFeature::NormalizedValueImageFeature(
    const std::string& name, float normalized_value)
: super(name, normalized_value) {}

void NormalizedValueImageFeature::Compute(
    InputVector<float>* resized_input_vector) const {
  (*resized_input_vector)[0] = value();
}


GrayImageFeature::GrayImageFeature(const std::string& name,
                                   const cv::Size& size,
                                   const cv::Mat& img_gray)
: super(name, GetPixelCount(size)) {
  cv::resize(img_gray, img_gray_, size);
}

const cv::Mat& GrayImageFeature::img_gray() const {
  return img_gray_;
}

void GrayImageFeature::Compute(InputVector<float>* resized_input_vector) const {
  std::size_t cur_ind = 0;
  for (int y = 0; y < img_gray_.rows; ++y)
    for (int x = 0; x < img_gray_.cols; ++x)
      (*resized_input_vector)[cur_ind++] = img_gray_.at<uchar>(y, x);
}


}  // namespace ann

}  // namespace slesann
