/*
 * trainer.cpp
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
 * trainer.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

//#include <fann_train.h>

#include <fstream>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>

#include "trainer.h"
#include "training_algorithm.h"
#include "fann_utils.h"

namespace slesann {

namespace ann {

namespace {

}  // namespace

Trainer::Trainer() : ann_(NULL) {}

Trainer::Trainer(fann* ann) : ann_(ann) {}

Trainer::Trainer(fann* ann, float bit_fail_limit,
                 const TrainingAlgorithm& algorithm,
                 float learning_rate, float learning_momentum)
: ann_(ann) {
  set_bit_fail_limit(bit_fail_limit);
  set_training_algorithm(algorithm);
  set_learning_rate(learning_rate);
  set_learning_momentum(learning_momentum);
}

void Trainer::Train(const InputVector<float>& input_vector,
                    const OutputVector<float>& output_vector) {
  tmp_input_vector_ = input_vector;
  tmp_output_vector_ = output_vector;
  fann_train(ann_, &tmp_input_vector_[0], &tmp_output_vector_[0]);
}

bool Trainer::Train(const AnnData& data, const float* random_weight_limit,
                    std::size_t max_epochs, std::size_t epochs_between_reports,
                    float desired_error,
                    float* mse, std::size_t* bit_fail) {
  if (random_weight_limit == NULL)
    fann_init_weights(ann_, data.data());
  else
    fann_randomize_weights(ann_, -*random_weight_limit, *random_weight_limit);
  fann_shuffle_train_data(data.data());
  fann_train_on_data(ann_, data.data(), max_epochs, epochs_between_reports,
                     desired_error);
  return GetMseAndBitFail(ann_, &mse, &bit_fail);
}

bool Trainer::Test(const AnnData& data, float* mse, std::size_t* bit_fail) {
  float mse_tmp = fann_test_data(ann_, data.data());
  BOOST_ASSERT(fabs(mse_tmp - fann_get_MSE(ann_)) < 0.0001);
  return GetMseAndBitFail(ann_, &mse, &bit_fail);
}

bool Trainer::Test(const InputVector<float>& input_vector,
          const OutputVector<float>& desired_output,
          float* square_error, std::size_t* bit_fail) {
  fann_reset_MSE(ann_);
  tmp_input_vector_ = input_vector;
  tmp_output_vector_ = desired_output;
  fann_test(ann_, &tmp_input_vector_[0], &tmp_output_vector_[0]);
  return GetMseAndBitFail(ann_, &square_error, &bit_fail);
}

fann* Trainer::ann() const {
  return ann_;
}

void Trainer::set_ann(fann* ann) {
  ann_ = ann;
}

float Trainer::bit_fail_limit() const {
  return fann_get_bit_fail_limit(ann_);
}

void Trainer::set_bit_fail_limit(float bit_fail_limit) {
  std::cerr << "Set bit fail limit to " << bit_fail_limit << std::endl;
  fann_set_bit_fail_limit(ann_, bit_fail_limit);
}

float Trainer::learning_rate() const {
  return fann_get_learning_rate(ann_);
}

void Trainer::set_learning_rate(float learning_rate) {
  std::cerr << "Set learning rate to " << learning_rate << std::endl;
  fann_set_learning_rate(ann_, learning_rate);
}

float Trainer::learning_momentum() const {
  return fann_get_learning_momentum(ann_);
}

void Trainer::set_learning_momentum(float learning_momentum) {
  std::cerr << "Set learning mementum to " << learning_momentum << std::endl;
  return fann_set_learning_momentum(ann_, learning_momentum);
}

void Trainer::set_training_algorithm(const TrainingAlgorithm& algorithm) {
  std::cerr << "Set training algo to " << algorithm.name() << std::endl;
  fann_set_training_algorithm(ann_, algorithm.train_enum());
}

TrainingAlgorithm Trainer::training_algorithm() const {
  const TrainingAlgorithm* algo =
      TrainingAlgorithm::Get(fann_get_training_algorithm(ann_));
  BOOST_ASSERT(algo != NULL); // should not happen
  return *algo;
}

}  // namespace ann

}  // namespace slesann
