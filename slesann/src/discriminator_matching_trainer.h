/*
 * discriminator_matching_trainer.h
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
 * discriminator_matching_trainer.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef DISCRIMINATOR_MATCHING_TRAINER_H_
#define DISCRIMINATOR_MATCHING_TRAINER_H_

#include "ann/matching_trainer.h"

namespace slesann {

namespace ann {

class TrainingAlgorithm;

template<typename T, bool cacheTransformations = true>
class DiscriminatorMatchingTrainer
: public MatchingTrainer<T, float, cacheTransformations> {
  typedef MatchingTrainer<T, float, cacheTransformations> super;
public:
  DiscriminatorMatchingTrainer(
      typename super::InputDataTransformerSharedPtr input_data_transformer,
      const typename super::Params& params)
  : super(input_data_transformer,
          typename super::OutputDataTransformerSharedPtr(
              new SigmoidSymmetricToProbabilityTransformer<float>()),
          params) {}

};

}  // namespace ann

}  // namespace slesann


#endif /* DISCRIMINATOR_MATCHING_TRAINER_H_ */
