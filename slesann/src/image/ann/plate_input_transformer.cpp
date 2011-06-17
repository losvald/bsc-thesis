/*
 * plate_input_transformer.cpp
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
 * plate_input_transformer.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include "../binary_image_utils.h"
#include "../plate_discriminator.h"
#include "image_feature.h"
#include "plate_input_transformer.h"


#include <opencv/highgui.h>

namespace slesann {

namespace ann {

namespace {

cv::Size kSize(150, 30);

}  // namespace

PlateInputTransformer::PlateInputTransformer()
: ImageInputTransformer(
    3 * PlateDiscriminator::kMaxCharactersCount +
    PlateDiscriminator::kVerticalProjectionN) { // FIXME
}

InputVector<float> PlateInputTransformer::operator()(
    const ImagePath& image_path) const {
  cv::Mat img_gray = PlateDiscriminator::LoadAsGrayscale(image_path);
  cv::Mat img_filtered = PlateDiscriminator::ApplyFilters(img_gray);
  cv::Mat img_filtered_trr = RemoveThinRegions(
      img_filtered, PlateDiscriminator::NonCharacterRegionsRemover(),
      PlateDiscriminator::kThinEdgeRemovalIterations);

  std::vector<Component> characters;
  PlateDiscriminator::ExtractCharacters(img_filtered_trr, &characters);

  std::vector<float> ver_bw_ratios;
  GetVerticalBlackWhiteRatios(img_filtered_trr,
                              PlateDiscriminator::kVerticalProjectionN,
                              &ver_bw_ratios);

  std::vector<InputVector<float> > features_ivs;

  for (std::size_t i = 0; i < ver_bw_ratios.size(); ++i) {
    features_ivs.push_back(NormalizedValueImageFeature("", ver_bw_ratios[i])());
  }

  for (std::size_t i = 0; i < PlateDiscriminator::kMaxCharactersCount; ++i) {
    float r = (i < characters.size() ? characters[i].rectangularity() : 0);
    features_ivs.push_back(NormalizedValueImageFeature("", r)());
    std::pair<float, float> c = (i < characters.size() ? characters[i].normalized_centroid<float>() : std::make_pair(0.f, 0.f));
    features_ivs.push_back(NormalizedValueImageFeature("", c.first)());
    features_ivs.push_back(NormalizedValueImageFeature("", c.second)());
  }

  InputVector<float> iv_merged(features_ivs);
  assert(iv_merged.size() == input_neuron_count());
  return iv_merged;
}

}  // namespace ann

}  // namespace slesann
