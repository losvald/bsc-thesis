/*
 * training_algorithm.cpp
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
 * training_algorithm.cpp
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#include "training_algorithm.h"

namespace slesann {

namespace ann {


const TrainingAlgorithm* TrainingAlgorithm::Get(fann_train_enum train_enum) {
  if (train_enum == TrainingAlgorithm::kIncremental.train_enum())
    return &TrainingAlgorithm::kIncremental;
  if (train_enum == TrainingAlgorithm::kBatch.train_enum())
    return &TrainingAlgorithm::kBatch;
  if (train_enum == TrainingAlgorithm::kRprop.train_enum())
    return &TrainingAlgorithm::kRprop;
  if (train_enum == TrainingAlgorithm::kQuickprop.train_enum())
    return &TrainingAlgorithm::kQuickprop;
  return NULL;
}

const TrainingAlgorithm* TrainingAlgorithm::Get(const std::string& algo_name) {
  if (algo_name == TrainingAlgorithm::kIncremental.name())
    return &TrainingAlgorithm::kIncremental;
  if (algo_name == TrainingAlgorithm::kBatch.name())
    return &TrainingAlgorithm::kBatch;
  if (algo_name == TrainingAlgorithm::kRprop.name())
    return &TrainingAlgorithm::kRprop;
  if (algo_name == TrainingAlgorithm::kQuickprop.name())
    return &TrainingAlgorithm::kQuickprop;
  return NULL;
}

std::vector<TrainingAlgorithm> TrainingAlgorithm::ListAll() {
  return std::vector<TrainingAlgorithm>(algorithms_, algorithms_ + 4);
}

fann_train_enum TrainingAlgorithm::train_enum() const {
  return train_enum_;
}

const std::string& TrainingAlgorithm::name() const {
  return name_;
}

TrainingAlgorithm::TrainingAlgorithm(const std::string& name,
                                     fann_train_enum train_enum)
: name_(name), train_enum_(train_enum) {}

const TrainingAlgorithm TrainingAlgorithm::kIncremental("incremental",
                                                        FANN_TRAIN_INCREMENTAL);
const TrainingAlgorithm TrainingAlgorithm::kBatch("batch",
                                                  FANN_TRAIN_BATCH);
const TrainingAlgorithm TrainingAlgorithm::kRprop("rprop",
                                                  FANN_TRAIN_RPROP);
const TrainingAlgorithm TrainingAlgorithm::kQuickprop("quickprop",
                                                      FANN_TRAIN_QUICKPROP);
const TrainingAlgorithm TrainingAlgorithm::algorithms_[] = {kIncremental,
                                                            kBatch,
                                                            kRprop,
                                                            kQuickprop};


}  // namespace ann

}  // namespace slesann
