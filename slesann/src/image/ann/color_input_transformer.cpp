/*
 * color_input_transformer.cpp
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
 * color_input_transformer.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#include "../boundary_detector.h"
#include "../color_discriminator.h"
#include "../geometry_utils.h"
#include "color_input_transformer.h"
#include "image_feature.h"

#include <opencv/cv.h>

namespace slesann {

namespace ann {

ColorInputTransformer::ColorInputTransformer()
: ImageInputTransformer(3 + // average bgr
                        0) {
}

InputVector<float> ColorInputTransformer::operator()(
    const ImagePath& image_path) const {

  cv::Mat img = ColorDiscriminator::LoadImage(image_path);
  Hull body_hull = FindGreatestHull(img,
                                    EmptyColorBoundaryDetector<kBgr>(img));

  cv::Mat img_hsv; cv::cvtColor(img, img_hsv, CV_BGR2HSV);

  std::vector<PixelPosition> body_pixels;
  ColorDiscriminator::GetBodyPixels(img_hsv, body_hull, &body_pixels);

  cv::Vec3b bgr_avg = ColorDiscriminator::GetAverageBgrColor(img, body_pixels);

  NormalizedValueImageFeature norm_blue("normalized blue", bgr_avg[0] / 255.f);
  NormalizedValueImageFeature norm_green("normalized green", bgr_avg[1] / 255.f);
  NormalizedValueImageFeature norm_red("normalized red", bgr_avg[2] / 255.f);

  std::vector<InputVector<float> > features_ivs;
  features_ivs.push_back(norm_blue());
  features_ivs.push_back(norm_green());
  features_ivs.push_back(norm_red());

  // TODO

  InputVector<float> iv_merged(features_ivs);
  assert(iv_merged.size() == input_neuron_count());
  return iv_merged;
}

}  // namespace ann

}  // namespace slesann
