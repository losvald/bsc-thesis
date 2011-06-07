/*
 * shape_matching_trainer_test.h
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
 * shape_matching_trainer_test.h
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#ifndef SHAPE_MATCHING_TRAINER_TEST_H_
#define SHAPE_MATCHING_TRAINER_TEST_H_

#include <vector>

#include <boost/filesystem/path.hpp>

namespace slesann {

namespace ann {

class TrainingParameters;

namespace test {

void TestLoadTrainingSet(
    const boost::filesystem::path& train_data_file_path,
    const boost::filesystem::path& test_data_file_path,
    const boost::filesystem::path& train_data_out_file_path,
    const boost::filesystem::path& test_data_out_file_path,
    const ShapeMatchingTrainer::Params& training_params);

}  // namespace test

}  // namespace ann

}  // namespace slesann

#endif /* SHAPE_MATCHING_TRAINER_TEST_H_ */
