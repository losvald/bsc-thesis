/*
 * custom_ann_test.cpp
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
 * custom_ann_test.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#include <iostream>
#include <vector>

#include <boost/foreach.hpp>

#include "../../src/ann/ann.h"
#include "../../src/ann/trainer.h"
#include "../../src/ann/training_algorithm.h"
#include "../../src/ann/training_parameters.h"

namespace slesann {

namespace ann {

namespace test {

void TestCustomAnn(
    const boost::filesystem::path& traindata_file_path,
    const boost::filesystem::path& testdata_file_path,
    std::size_t input_neuron_count, std::size_t output_neuron_count,
    const TrainingParameters& training_params) {

  Ann ann(input_neuron_count, output_neuron_count,
          training_params.hidden_neuron_counts());
  ann.Save("/tmp/custom_ann.start");

  Trainer trainer(ann.ann(),
                  training_params.bit_fail_limit(),
                  training_params.training_algo(),
                  training_params.learning_rate(),
                  training_params.learning_momentum());
//  trainer.set_training_algorithm(training_params.training_algo());
//  trainer.set_learning_rate(training_params.learning_rate());
//  trainer.set_learning_momentum(training_params.learning_momentum());

  std::cerr << "Weight init: ";
  if (training_params.random_weight_limit() == NULL)
    std::cerr << "auto" << std::endl;
  else
    std::cerr << "random(" << -*training_params.random_weight_limit()
    << ", " << *training_params.random_weight_limit() << std::endl;

  std::cerr << "Algorithm: " << trainer.training_algorithm().name() << std::endl;
  std::cerr << "bit fail: " << fann_get_bit_fail(ann.ann()) << std::endl;

  AnnData train_data(traindata_file_path);
  AnnData test_data(testdata_file_path);
//  Data test_data(testdata_file_path);

  fann_init_weights(ann.ann(), train_data.data());
  ann.Save("/tmp/custom_ann.mid");

  std::cout << "Training ANN...";
  bool training_ok = trainer.Train(train_data,
                                   training_params.random_weight_limit(),
                                   training_params.max_epochs(),
                                   training_params.epochs_between_reports(),
                                   training_params.desired_error());
  std::cout << (training_ok ? "PASSED" : "FAILED") << std::endl;

  std::cout << "Running test data...";
  float mse; std::size_t bit_fail;
  bool test_ok = trainer.Test(test_data, &mse, &bit_fail);
  std::cout << (test_ok ? "PASSED" : "FAILED") << std::endl;
  std::cout << "test data MSE = " << mse << std::endl;
  std::cout << "test data bit fail = " << bit_fail << std::endl;

  ann.Save("/tmp/custom_ann.end");

//  fann_reset_MSE(ann.ann());
//  std::cerr << "mse = " << fann_get_MSE(ann.ann()) << std::endl;
//  std::cerr << "bit fail = " << fann_get_bit_fail(ann.ann()) << std::endl;
//  InputVector<float> iv(2);
//  OutputVector<float> ov(1);
//  for (float a = 0.1; a <= 0.9; a += 0.3) {
//    for (float b = 0.1; b <= 0.9 && a + b <= 1; b += 0.4) {
//      iv[0] = a; iv[1] = b;
//
//      ann.Run(iv, &ov);
//      std::cerr << "run(" << iv << ") = " << ov[0] << "(" << a + b << ")\n";
//
//      Ann tann(ann.input_neuron_count(),
//               ann.output_neuron_count(),
//               training_params.hidden_neuron_counts());
//      tann.set_ann(trainer.ann());
//      tann.Run(iv, &ov);
//      std::cerr << "run(" << iv << ") = " << ov[0] << "(" << a + b << ")\n";
//
//      ov[0] = a + b;
//      trainer.Test(iv, ov);
//      std::cerr << "cur. mse = " << fann_get_MSE(ann.ann()) << std::endl;
//      std::cerr << "cur. bit fail = " << fann_get_bit_fail(ann.ann()) << std::endl;
//    }
//  }
//  std::cerr << "mse = " << fann_get_MSE(ann.ann()) << std::endl;
//  std::cerr << "bit fail = " << fann_get_bit_fail(ann.ann()) << std::endl;
}

}  // namespace test

}  // namespace ann

}  // namespace slesann
