/*
 * dummy_descriptor_matcher.h
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
 * dummy_descriptor_matcher.h
 *
 *  Created on: Apr 5, 2011
 *      Author: losvald
 */

#ifndef DUMMY_DESCRIPTOR_MATCHER_H_
#define DUMMY_DESCRIPTOR_MATCHER_H_

#include <cstdlib>
#include <cmath>

#include "../descriptor_matcher.h"
#include "dummy_base.h"

#include <boost/foreach.hpp>

namespace slesann {

class DummyDescriptorMatcher
: public DescriptorMatcher<DummyVehicleDescriptor> {
protected:
  float ComputeSimilarity(const DummyVehicleDescriptor& d1,
                          const DummyVehicleDescriptor& d2) const {
    DummyVehicleDescriptor sum = abs(d1) + abs(d2);
    DummyVehicleDescriptor diff = abs(abs(d1) - abs(d2));
    return (float) (sum - diff) / sum;
  }

  float ComputeUniqueness(
      const DummyVehicleDescriptor& d,
      const std::vector<float>& right_side_match_prob) const {

    bool any_above_threshold = false;
    BOOST_FOREACH(float p, right_side_match_prob) {
      any_above_threshold |= (p >= 0.9);
    }
    return 0;
  }
};

}  // namespace slesann

#endif /* DUMMY_DESCRIPTOR_MATCHER_H_ */
