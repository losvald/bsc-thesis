/*
 * default_vehicle_matcher.cpp
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
 * default_vehicle_matcher.cpp
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#include "default_vehicle_matcher.h"
#include "frame_info.h"

namespace slesann {

float DefaultVehicleMatcher::ComputeSimilarity(const FrameInfo& d1,
                                               const FrameInfo& d2) const {
  return med_color_disc_.ComputeSimilarity(d1.body_image_path(),
                                           d2.body_image_path());
}

float DefaultVehicleMatcher::ComputeUniqueness(
    const FrameInfo& d, const std::vector<float>& right_side_match_prob) const {
  return med_color_disc_.ComputeUniqueness(d.body_image_path(),
                                           right_side_match_prob);
}

}  // namespace slesann
