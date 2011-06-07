/*
 * median_color_discriminator.h
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
 * median_color_discriminator.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef MEDIAN_COLOR_DISCRIMINATOR_H_
#define MEDIAN_COLOR_DISCRIMINATOR_H_

#include "../discriminator.h"
#include "image_base.h"

namespace slesann {

class MedianColorDiscriminator : public Discriminator<ImagePath> {

public:
  float ComputeSimilarity(const ImagePath& d1, const ImagePath& d2) const;
  float ComputeUniqueness(const ImagePath& d,
                          const std::vector<float>& match_candidates) const;
};

}  // namespace slesann

#endif /* MEDIAN_COLOR_DISCRIMINATOR_H_ */
