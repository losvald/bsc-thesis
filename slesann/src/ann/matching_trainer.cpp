/*
 * matching_trainer.cpp
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
 * matching_trainer.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#include "matching_trainer.h"
#include "training_parameters.h"

namespace slesann {

namespace ann {

//MatchingTrainerParameters::MatchingTrainerParameters(
//    const TrainingParameters& training_params)
//: hidden_neuron_counts_(training_params.hidden_neuron_counts()),
//  random_weight_limit_(training_params.random_weight_limit()),
//  max_epochs_(training_params.max_epochs()),
//  epochs_between_reports_(training_params.epochs_between_reports()),
//  desired_error_(training_params.desired_error()),
//  bit_fail_limit_(training_params.bit_fail_limit()),
//  training_algo_(&training_params.training_algo()),
//  learning_rate_(training_params.learning_rate()),
//  learning_momentum_(training_params.learning_momentum()) {}
//
//MatchingTrainerParameters::MatchingTrainerParameters(
//    const std::vector<std::size_t>& hidden_neuron_counts,
//    const float* random_weight_limit,
//    std::size_t max_epochs,
//    std::size_t epochs_between_reports,
//    float desired_error,
//    float bit_fail_limit,
//    const TrainingAlgorithm* training_algo,
//    float learning_rate,
//    float learning_momentum)
//: hidden_neuron_counts_(hidden_neuron_counts),
//  random_weight_limit_(random_weight_limit),
//  max_epochs_(max_epochs),
//  epochs_between_reports_(epochs_between_reports),
//  desired_error_(desired_error),
//  bit_fail_limit_(bit_fail_limit),
//  training_algo_(training_algo),
//  learning_rate_(learning_rate),
//  learning_momentum_(learning_momentum) {}
//
//const std::vector<std::size_t>&
//MatchingTrainerParameters::hidden_neuron_counts() const {
//  return hidden_neuron_counts_;
//}
//
//const float* MatchingTrainerParameters::random_weight_limit() const {
//  return random_weight_limit_;
//}
//
//std::size_t MatchingTrainerParameters::max_epochs() const {
//  return max_epochs_;
//}
//
//std::size_t MatchingTrainerParameters::epochs_between_reports() const {
//  return epochs_between_reports_;
//}
//
//float MatchingTrainerParameters::desired_error() const {
//  return desired_error_;
//}
//
//float MatchingTrainerParameters::bit_fail_limit() const {
//  return bit_fail_limit_;
//}
//
//const TrainingAlgorithm& MatchingTrainerParameters::training_algo() const {
//  return *training_algo_;
//}
//
//float MatchingTrainerParameters::learning_rate() const {
//  return learning_rate_;
//}
//
//float MatchingTrainerParameters::learning_momentum() const {
//  return learning_momentum_;
//}

}  // namespace ann

}  // namespace slesann
