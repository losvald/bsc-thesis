/*
 * discriminator.h
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
 * discriminator.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef DISCRIMINATOR_H_
#define DISCRIMINATOR_H_

#include <vector>

#include <boost/shared_ptr.hpp>

namespace slesann {

template<typename T>
class Discriminator {
public:
  typedef boost::shared_ptr<Discriminator> SharedPtr;

  /**
   * Computes the probability that two descriptors describe the same vehicle.
   *
   * @param d1 a descriptor from the left side
   * @param d2 a descriptor from the right side
   * @return a real number from the range [0, 1]
   */
  virtual float ComputeSimilarity(const T& d1, const T& d2) const = 0;

  /**
   * Returns the probability that d does not
   * match any element from match_candidates vector.
   *
   * @param d a descriptor from the left side
   * @param match_candidates the vector containing probabilities
   * that d matches each element from match candidates
   * (this vector is computed using the ComputeSimilarity() method)
   */
  virtual float ComputeUniqueness(const T& d,
                                  const std::vector<float>& match_candidates) const = 0;
};

}  // namespace slesann

#endif /* DISCRIMINATOR_H_ */
