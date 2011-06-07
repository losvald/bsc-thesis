/*
 * shape_matching_trainer_test.cpp
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
 * shape_matching_trainer_test.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#include <vector>

#include <boost/foreach.hpp>

#include "../../../src/ann/training_algorithm.h"
#include "../../../src/ann/training_parameters.h"
#include "../../../src/image/ann/discriminator_training_io.h"
#include "../../../src/image/ann/shape_matching_trainer.h"

#include "../../../src/ann/trainer.h"

#include "shape_matching_trainer_test.h"

namespace slesann {

namespace ann {

namespace test {

namespace {



}  // namespace

void TestLoadTrainingSet(
    const boost::filesystem::path& train_data_file_path,
    const boost::filesystem::path& test_data_file_path,
    const boost::filesystem::path& train_data_out_file_path,
    const boost::filesystem::path& test_data_out_file_path,
    const ShapeMatchingTrainer::Params& training_params) {
  ShapeMatchingTrainer smt(training_params);

  LoadTrainDataSet(train_data_file_path, &smt);
  LoadTestDataSet(test_data_file_path, &smt);
  std::cout << "Shape matching trainer:" << std::endl;
  std::cout << smt << std::endl;

  typedef ShapeMatchingTrainer::AnnMapping AnnMapping;
  std::vector<AnnMapping> ann_mappings = smt.ann_train_mappings();

  std::cout << "Ann mappings:\n";
  BOOST_FOREACH(const AnnMapping& m, ann_mappings) {
    std::cout << m.first << std::endl << "-> " << std::endl
        << m.second << std::endl;
  }

  smt.CreateTrainingFile(train_data_out_file_path);
  smt.CreateTestFile(test_data_out_file_path);

  AnnData train_data(train_data_out_file_path);
  train_data.Save("/tmp/train-data.save");
  AnnData test_data(test_data_out_file_path);
  test_data.Save("/tmp/test-data.save");
}


}  // namespace test

}  // namespace ann

}  // namespace slesann
