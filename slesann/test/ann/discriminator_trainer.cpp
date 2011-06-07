/*
 * discriminator_trainer.cpp
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
 * discriminator_trainer.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include "../test.h"

#ifndef SKIP_TESTS

#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include <boost/foreach.hpp>

#include "../../src/ann/matching_trainer.h"
#include "../../src/ann/training_algorithm.h"
#include "../../src/discriminator_matching_trainer.h"

namespace {

using namespace slesann;
using namespace slesann::ann;

class MyInputDataTransformer : public InputDataTransformer<int, float> {
public:
  typedef InputDataTransformer<int, float> super;
  MyInputDataTransformer() : super(1) {}

  InputVector<float> operator()(const int& data) const {
    InputVector<float> ret(super::input_neuron_count());
    std::cerr << '[' << data << ']';
    ret[0] = data / 10.0f;
    return ret;
  }

};

class MyOutputDataTransformer : public OutputDataTransformer<float, float> {
public:
  typedef OutputDataTransformer<float, float> super;

  MyOutputDataTransformer() : super(1) {}

  OutputVector<float> operator()(const float& data) const {
    OutputVector<float> ret(super::output_neuron_count());
    ret[0] = data / 10.f;
    return ret;
  }

  float operator()(const OutputVector<float>& output_vector) const {
    return output_vector[0] * 10.f;
  }

};

typedef MatchingTrainer<int, float> MyMatchingTrainer;

typedef DiscriminatorMatchingTrainer<int> MyDiscriminatorMatchingTrainer;

std::vector<std::size_t> HiddenNeuronCounts() {
  std::vector<std::size_t> ret(1);
  ret[0] = 1;
  return ret;
}

MyMatchingTrainer::Params CreateParams() {
  return MyMatchingTrainer::Params(HiddenNeuronCounts(),
                                   NULL,
                                   kDefaultMaxEpochs,
                                   kDefaultEpochsBetweenReports,
                                   0.f,
                                   kDefaultBitFailLimit,
                                   &TrainingAlgorithm::kRprop,
                                   kDefaultLearningRate,
                                   kDefaultLearningMomentum);
}

}

TEST(ann, matching_trainer01) {
  MyMatchingTrainer mt(
      MyInputDataTransformer::SharedPtr(new MyInputDataTransformer()),
      MyOutputDataTransformer::SharedPtr(new MyOutputDataTransformer()),
      CreateParams()
  );

  mt.AddForTrain(1, 2, 1.5);
  mt.AddForTrain(2, 4, 3);
  mt.AddForTrain(3, 4, 3.5);
  mt.AddForTrain(1, 4, 2.5);
  std::cerr << mt << std::endl;

  std::vector<MyMatchingTrainer::AnnMapping> mappings = mt.ann_train_mappings();
  BOOST_FOREACH(const MyMatchingTrainer::AnnMapping& m, mappings) {
    std::cerr << "AnnMaping:\n" << m.first << "\n" << m.second << std::endl;
  }
}

TEST(ann, discriminator_trainer01) {
  MyDiscriminatorMatchingTrainer mt(
      MyInputDataTransformer::SharedPtr(new MyInputDataTransformer()),
      CreateParams()
  );

  mt.AddForTrain(1, 2, 1.5f);
}

#endif
