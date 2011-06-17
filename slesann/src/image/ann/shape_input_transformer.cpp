/*
 * shape_input_transformer.cpp
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
 * shape_input_transformer.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#include <cassert>

#include <boost/foreach.hpp>

#include "../boundary_detector.h"
#include "../geometry_utils.h"
#include "../image_loader.h"
#include "../shape_discriminator.h"
#include "image_feature.h"
#include "shape_input_transformer.h"

namespace slesann {

namespace ann {

namespace {


}  // namespace

ShapeInputTransformer::ShapeInputTransformer()
: ImageInputTransformer(1 + // perimeter quotient
                        1 + // height / perimeter
                        1 + //side_function_median
                        1 + // height_perimeter_quotient
                        1 + // width_perimeter_quotient
                        0) {
}

InputVector<float> ShapeInputTransformer::operator()(
    const ImagePath& image_path) const {

  cv::Mat img = ShapeDiscriminator::LoadImage(image_path);
  Hull body_hull = FindGreatestHull(img, EmptyColorBoundaryDetector<kBgr>(img));



  const NormalizedValueImageFeature perimeter_quotient(
      "perimeter quotient",
      ShapeDiscriminator::GetPerimeterQuotient(body_hull));

  const NormalizedValueImageFeature width_perimeter_quotient(
      "width / perimeter",
      ShapeDiscriminator::GetWidthPerimeterQuotient(body_hull)
  );

  const NormalizedValueImageFeature height_perimeter_quotient(
      "height / perimeter",
      ShapeDiscriminator::GetHeightPerimeterQuotient(body_hull)
  );

  const NormalizedValueImageFeature side_function_median(
      "side function median",
      ShapeDiscriminator::GetSideFunctionMedian(body_hull)
  );

  const NormalizedValueImageFeature normalized_height(
      "",
      ShapeDiscriminator::GetNormalizedHeight(body_hull)
  );

  std::vector<InputVector<float> > features_ivs;
  features_ivs.push_back(perimeter_quotient());
  features_ivs.push_back(width_perimeter_quotient());
  features_ivs.push_back(side_function_median());
  features_ivs.push_back(height_perimeter_quotient());
  features_ivs.push_back(normalized_height());

  // TODO

  InputVector<float> iv_merged(features_ivs);
  assert(iv_merged.size() == input_neuron_count());
  return iv_merged;
}

}  // namespace anne

}  // namespace slesann
