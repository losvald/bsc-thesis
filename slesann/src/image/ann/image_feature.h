/*
 * image_feature.h
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
 * image_feature.h
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#ifndef IMAGE_FEATURE_H_
#define IMAGE_FEATURE_H_

#include <iostream>
#include <string>

#include "image_input_transformer.h"

namespace slesann {

namespace ann {

class ImageFeature {
public:
  ImageFeature(const std::string& name, std::size_t input_vector_size);
  InputVector<float> operator()() const;
  friend std::ostream& operator<<(std::ostream& os, const ImageFeature& f);
  const std::string& name() const;
protected:
  virtual void Compute(InputVector<float>* resized_input_vector) const = 0;
  void ClearCache() const;
private:
  std::string name_;
  std::size_t input_vector_size_;
  mutable InputVector<float> input_vector_cache_;
};

class OneValueImageFeature : public ImageFeature {
  typedef ImageFeature super;
public:
  OneValueImageFeature(const std::string& name, float value);
  float value() const;
protected:
  virtual void Compute(InputVector<float>* resized_input_vector) const = 0;
private:
  float value_;
};

class NormalizedValueImageFeature : public OneValueImageFeature {
  typedef OneValueImageFeature super;
public:
  NormalizedValueImageFeature(const std::string& name, float normalized_value);
protected:
  void Compute(InputVector<float>* resized_input_vector) const;
};

class GrayImageFeature : public ImageFeature {
  typedef ImageFeature super;
public:
  GrayImageFeature(const std::string& name, const cv::Size& size,
                   const cv::Mat& img_gray);
  const cv::Mat& img_gray() const;
protected:
  void Compute(InputVector<float>* resized_input_vector) const;
private:
  cv::Mat img_gray_;
};

}  // namespace ann

}  // namespace slesann

#endif /* IMAGE_FEATURE_H_ */
