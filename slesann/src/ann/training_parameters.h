/*
 * training_parameters.h
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
 * training_parameters.h
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#ifndef TRAINING_PARAMETERS_H_
#define TRAINING_PARAMETERS_H_

#include <vector>

namespace slesann {

namespace ann {

class TrainingAlgorithm;

class TrainingParameters {
public:
  TrainingParameters(//std::size_t input_neuron_count,
                     //std::size_t output_neuron_count,
                     const std::vector<std::size_t>& hidden_layer_neuron_counts,
                     const float* random_weight_limit,
                     std::size_t max_epochs,
                     std::size_t epochs_between_reports,
                     float desired_error,
                     float bit_fail_limit,
                     const TrainingAlgorithm* training_algo,
                     float learning_rate,
                     float learning_momentum);

//  std::size_t input_neuron_count() const;
//  std::size_t output_neuron_count() const;
  const std::vector<std::size_t>& hidden_neuron_counts() const;
  const float* random_weight_limit() const;
  std::size_t max_epochs() const;
  std::size_t epochs_between_reports() const;
  float desired_error() const;
  float bit_fail_limit() const;
  const TrainingAlgorithm& training_algo() const;
  float learning_rate() const;
  float learning_momentum() const;

private:
  std::size_t input_neuron_count_;
  std::size_t output_neuron_count_;
  std::vector<std::size_t> hidden_neuron_counts_;
  const float* random_weight_limit_;
  std::size_t max_epochs_;
  std::size_t epochs_between_reports_;
  float desired_error_;
  float bit_fail_limit_;
  const TrainingAlgorithm* training_algo_;
  float learning_rate_;
  float learning_momentum_;
};

}  // namespace ann

}  // namespace slesann

#endif /* TRAINING_PARAMETERS_H_ */
