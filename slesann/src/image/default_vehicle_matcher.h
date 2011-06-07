/*
 * default_vehicle_matcher.h
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
 * default_vehicle_matcher.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef DEFAULT_VEHICLE_MATCHER_H_
#define DEFAULT_VEHICLE_MATCHER_H_

#include "../descriptor_matcher.h"
#include "frame_info.h"
#include "median_color_discriminator.h"

namespace slesann {

//class FrameInfo;

class DefaultVehicleMatcher : public DescriptorMatcher<FrameInfo> {
public:
//  std::vector<Discriminator::SharedPtr> discriminators;
  MedianColorDiscriminator med_color_disc_;

  float ComputeSimilarity(const FrameInfo& d1, const FrameInfo& d2) const;

  float ComputeUniqueness(
      const FrameInfo& d,
      const std::vector<float>& right_side_match_prob) const;
};

}  // namespace slesann

#endif /* DEFAULT_VEHICLE_MATCHER_H_ */
