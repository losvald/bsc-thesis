/*
 * custom_ann_test.h
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
 * custom_ann_test.h
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#ifndef CUSTOM_ANN_TEST_H_
#define CUSTOM_ANN_TEST_H_

#include <vector>

#include <boost/filesystem/path.hpp>

namespace slesann {

namespace ann {

namespace test {

void TestCustomAnn(
    const boost::filesystem::path& traindata_file_path,
    const boost::filesystem::path& testdata_file_path,
    std::size_t input_neuron_count, std::size_t output_neuron_count,
    const TrainingParameters& training_params);

}  // namespace test

}  // namespace ann

}  // namespace slesann

#endif /* CUSTOM_ANN_TEST_H_ */
