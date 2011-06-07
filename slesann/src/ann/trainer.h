/*
 * trainer.h
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
 * trainer.h
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#ifndef ANN_TRAINER_H_
#define ANN_TRAINER_H_

#include <utility>
#include <vector>

//#include <boost/filesystem/path.hpp>

#include <fann.h>

#include "data.h"
#include "input_vector.h"
#include "output_vector.h"

namespace slesann {

namespace ann {

class AnnData;
class TrainingAlgorithm;

const float kDefaultLearningRate = 0.7;
const float kDefaultLearningMomentum = 0.f;
const float kDefaultBitFailLimit = 0.35f;
const float kDefaultDesiredError = 0.f;

const std::size_t kDefaultMaxEpochs = 1000;
const std::size_t kDefaultEpochsBetweenReports = 10;

class Trainer {
public:
  typedef std::pair<InputVector<float>, OutputVector<float> > AnnMapping;

  Trainer();

  Trainer(fann* ann);

  Trainer(fann* ann, float bit_fail_limit,
          const TrainingAlgorithm& algorithm,
          float learning_rate, float learning_momentum);

  void Train(const InputVector<float>& input_vector,
             const OutputVector<float>& output_vector);

  bool Train(const AnnData& data, const float* random_weight_limit,
             std::size_t max_epochs, std::size_t epochs_between_reports,
             float desired_error, float* mse = NULL,
             std::size_t* bit_fail = NULL);

  bool Test(const AnnData& data, float* mse = NULL,
            std::size_t* bit_fail = NULL);

  bool Test(const InputVector<float>& input_vector,
            const OutputVector<float>& desired_output,
            float* square_error = NULL, std::size_t* bit_fail = NULL);

  fann* ann() const;
  void set_ann(fann* ann);
  float bit_fail_limit() const;
  void set_bit_fail_limit(float bit_fail_limit);
  float learning_rate() const;
  void set_learning_rate(float learning_rate);
  float learning_momentum() const;
  void set_learning_momentum(float learning_momentum);
  void set_training_algorithm(const TrainingAlgorithm& algorithm);
  TrainingAlgorithm training_algorithm() const;
private:
  mutable fann* ann_;
  mutable std::vector<float> tmp_input_vector_;
  mutable std::vector<float> tmp_output_vector_;
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_TRAINER_H_ */
