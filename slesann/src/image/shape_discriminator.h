/*
 * shape_discriminator.h
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
 * shape_discriminator.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef SHAPE_DISCRIMINATOR_H_
#define SHAPE_DISCRIMINATOR_H_

#include "../discriminator.h"
#include "image_base.h"
#include "geometry_utils.h"

namespace slesann {

class ShapeDiscriminator : public Discriminator<ImagePath> {

public:
  static const float kResizeFactor;

  float ComputeSimilarity(const ImagePath& img_path1,
                          const ImagePath& img_path2) const;
  float ComputeUniqueness(const ImagePath& img_path,
                          const std::vector<float>& match_candidates) const;

  static cv::Mat LoadImage(const ImagePath& img_path);

  static float GetSideFunctionMedian(const Hull& h);

  static float GetSideFunctionAverage(const Hull& h);

  static float ComputeWidthHeightRatio(const Hull& h);

  static float GetNormalizedHeight(const Hull& h);

  static float GetHeightPerimeterQuotient(const Hull& h);

  static float GetWidthPerimeterQuotient(const Hull& h);

  static float GetPerimeterQuotient(const Hull& h);

  static float GetRectangularity(const Hull& h);

//  static InputVector<float>
};

}  // namespace slesann

#endif /* SHAPE_DISCRIMINATOR_H_ */
