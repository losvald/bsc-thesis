/*
 * descriptor_matcher.h
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
 * matcher.h
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#ifndef MATCHER_H_
#define MATCHER_H_

#include <vector>
#include <utility>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include "discriminator.h"
#include "matching/assignment.h"

namespace slesann {

template<typename T>
class DescriptorMatcher : public Discriminator<T> {
public:
  typedef boost::shared_ptr< DescriptorMatcher<T> > SharedPtr;
  typedef T DescriptorType;
  typedef std::pair<T, T> MatchedPair;

  /**
   * Returns the list of matched descriptors along with matched probabilities.
   *
   * Only true matches are returned, i.e. no descriptor
   * which is considered unique is contained in any of the pairs.
   *
   * @param descriptor list which should be matched
   * @param another descriptor list to which the first one should be matched
   *
   * @return the list of pairs (matched_pair, probability)
   */
  std::vector< std::pair<MatchedPair, float> > Match(
      const std::vector<T>& left_side,
      const std::vector<T>& right_side) const;

//protected:
  /**
   * Computes the probability that two descriptors describe the same vehicle.
   *
   * @param d1 a descriptor from the left side
   * @param d2 a descriptor from the right side
   * @return a real number from the range [0, 1]
   */
  virtual float ComputeSimilarity(const T& d1, const T& d2) const = 0;

  /**
   * Returns the probability that the given descriptor from the left side
   * does not describe any vehicle described by any descriptors from
   * the right side.
   *
   * @param d a descriptor from the left side
   * @param right_side_match_prob the vector containing probabilities
   * that the descriptor matches each of the descriptors from the right side
   * (this vector is computed using the ComputeSimilarity() method)
   */
  virtual float ComputeUniqueness(const T& d,
                                  const std::vector<float>& right_side_match_prob) const = 0;

};

template<typename T>
std::vector< std::pair<typename DescriptorMatcher<T>::MatchedPair, float> >
DescriptorMatcher<T>::Match(const std::vector<T>& left_side,
                            const std::vector<T>& right_side) const {
  // build probability matrix
  // rows are descriptors at start checkpoint
  // columns are descriptors at end checkpoint
  // row/column indexes correspond to vector indexes
  // for indexes in vector bound other indexes represent dummy descriptors,
  // so that descriptors can match to those in the worst case
  std::size_t first_unique_index = right_side.size();
  matching::ProbabilityMatrix prob_matrix(
      left_side.size(),
      std::vector<float>(right_side.size(), 0.f));
  // compute probabilities for match
  for (std::size_t r = 0; r < left_side.size(); ++r) {
    for (std::size_t c = 0; c < right_side.size(); ++c) {
      prob_matrix[r][c] = ComputeSimilarity(left_side[r], right_side[c]);
    }
  }
  // compute probabilities for uniqueness
  for (std::size_t r = 0; r < left_side.size(); ++r) {
    prob_matrix[r][left_side.size() + r] =
        ComputeUniqueness(left_side[r], prob_matrix[r]);
  }

  using namespace matching;
  Assignment tmp_assignment = matching::MatchWithMaxProbability(prob_matrix);
  Assignment no_unique_assignment;
  BOOST_FOREACH(const AssignmentPair& p, tmp_assignment) {
    if (p.second < (int)first_unique_index)
      no_unique_assignment.push_back(p);
  }

  typedef std::pair<MatchedPair, float> MatchEntry;
  std::vector<MatchEntry> ret;
  ret.reserve(no_unique_assignment.size());
  BOOST_FOREACH(const AssignmentPair& p, no_unique_assignment) {
    ret.push_back(std::make_pair(
        MatchedPair(left_side[p.first], right_side[p.second]),
        prob_matrix[p.first][p.second]));
  }
  return ret;
}

}  // namespace slesann

#endif /* MATCHER_H_ */
