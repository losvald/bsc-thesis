/*
 * training_parameters.cpp
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
 * training_parameters.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#include "training_parameters.h"
#include "training_algorithm.h"

namespace slesann {

namespace ann {

TrainingParameters::TrainingParameters(
    //std::size_t input_neuron_count,
    //std::size_t output_neuron_count,
    const std::vector<std::size_t>& hidden_neuron_counts,
    const float* random_weight_limit,
    std::size_t max_epochs,
    std::size_t epochs_between_reports,
    float desired_error,
    float bit_fail_limit,
    const TrainingAlgorithm* training_algo,
    float learning_rate,
    float learning_momentum)
: //input_neuron_count_(input_neuron_count),
  //output_neuron_count_(output_neuron_count),
  hidden_neuron_counts_(hidden_neuron_counts),
  random_weight_limit_(random_weight_limit),
  max_epochs_(max_epochs),
  epochs_between_reports_(epochs_between_reports),
  desired_error_(desired_error),
  bit_fail_limit_(bit_fail_limit),
  training_algo_(training_algo),
  learning_rate_(learning_rate),
  learning_momentum_(learning_momentum) {}

//std::size_t TrainingParameters::input_neuron_count() const {
//  return input_neuron_count_;
//}
//
//std::size_t TrainingParameters::output_neuron_count() const {
//  return output_neuron_count_;
//}

const std::vector<std::size_t>&
TrainingParameters::hidden_neuron_counts() const {
  return hidden_neuron_counts_;
}

const float* TrainingParameters::random_weight_limit() const {
  return random_weight_limit_;
}

std::size_t TrainingParameters::max_epochs() const {
  return max_epochs_;
}

std::size_t TrainingParameters::epochs_between_reports() const {
  return epochs_between_reports_;
}

float TrainingParameters::desired_error() const {
  return desired_error_;
}

float TrainingParameters::bit_fail_limit() const {
  return bit_fail_limit_;
}

const TrainingAlgorithm& TrainingParameters::training_algo() const {
  return *training_algo_;
}

float TrainingParameters::learning_rate() const {
  return learning_rate_;
}

float TrainingParameters::learning_momentum() const {
  return learning_momentum_;
}

}  // namespace ann

}  // namespace slesann
