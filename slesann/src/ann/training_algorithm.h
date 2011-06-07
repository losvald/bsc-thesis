/*
 * training_algorithm.h
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
 * training_algorithm.h
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#ifndef ANN_TRAINING_ALGORITHM_H_
#define ANN_TRAINING_ALGORITHM_H_

#include <string>
#include <vector>

#include <fann.h>

namespace slesann {

namespace ann {

class TrainingAlgorithm {
public:
  static const TrainingAlgorithm kIncremental;
  static const TrainingAlgorithm kBatch;
  static const TrainingAlgorithm kRprop;
  static const TrainingAlgorithm kQuickprop;

  static const TrainingAlgorithm* Get(fann_train_enum train_enum);
  static const TrainingAlgorithm* Get(const std::string& algo_name);
  static std::vector<TrainingAlgorithm> ListAll();

  fann_train_enum train_enum() const;
  const std::string& name() const;
private:
  static const TrainingAlgorithm algorithms_[];

  TrainingAlgorithm(const std::string& name, fann_train_enum enum_train);

  std::string name_;
  fann_train_enum train_enum_;
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_TRAINING_ALGORITHM_H_ */
