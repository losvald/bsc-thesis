/*
 * data_builder.h
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
 * data_builder.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef DATA_BUILDER_H_
#define DATA_BUILDER_H_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/filesystem/path.hpp>

#include "input_vector.h"
#include "output_vector.h"

namespace slesann {

namespace ann {

class DataBuilder {
  std::size_t input_neuron_count_;
  std::size_t output_neuron_count_;
  mutable std::ofstream tmp_of_;
  std::size_t pair_count_;
  std::string tmp_file_name_;

public:
  DataBuilder(std::size_t input_neuron_count, std::size_t output_neuron_count);
  ~DataBuilder();

  template<typename T, typename U>
  void Append(const InputVector<T>& in, const OutputVector<U>& out) {
    tmp_of_ << in << std::endl << out << std::endl;
    ++pair_count_;
  }

  void Save(const boost::filesystem::path& file_path) const;

  friend std::ostream& operator<<(std::ostream& os, const DataBuilder& b);
};

}  // namespace ann

}  // namespace slesann

#endif /* DATA_BUILDER_H_ */
