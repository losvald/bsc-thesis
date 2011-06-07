/*
 * data_builder.cpp
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
 * data_builder.cpp
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#include "data_builder.h"
#include "../util/string_utils.h"

#include <boost/filesystem.hpp>

namespace slesann {

namespace ann {

namespace {

std::string NewTempFile() {
  static const std::string kTmpFileBaseName = "data_builder";
  static int cur_suffix = 0;
  return MakeString() << kTmpFileBaseName << cur_suffix++ << ".tmp";
}

}  // namespace

DataBuilder::DataBuilder(std::size_t input_neuron_count,
                         std::size_t output_neuron_count)
: input_neuron_count_(input_neuron_count),
  output_neuron_count_(output_neuron_count),
  pair_count_(0) {
  tmp_file_name_ = (boost::filesystem::initial_path() / NewTempFile()).string();
  tmp_of_.open(tmp_file_name_.c_str());
}

DataBuilder::~DataBuilder() {
  tmp_of_.close();
  boost::filesystem::remove(tmp_file_name_);
}

void DataBuilder::Save(const boost::filesystem::path& file_path) const {
  std::ofstream of;
  of.open(file_path.string().c_str());
  of << *this;
  of.close();
}

std::ostream& operator<<(std::ostream& os, const DataBuilder& b) {
  b.tmp_of_.flush();
  std::ifstream ifs;
  ifs.open(b.tmp_file_name_.c_str());
  os << b.pair_count_ << ' ' << b.input_neuron_count_ << ' ' <<
      b.output_neuron_count_ << std::endl;
  for (std::size_t i = 0; i < b.pair_count_; ++i) {
    // XXX ????? results in wrong order of input
   //    os << InputVector<std::string>::Input(b.input_neuron_count_, &ifs) <<
   //        std::endl <<
   //        OutputVector<std::string>::Input(b.output_neuron_count_, &ifs) <<
   //        std::endl;
    os << InputVector<std::string>::Input(b.input_neuron_count_, &ifs) <<
        std::endl;
    os << OutputVector<std::string>::Input(b.output_neuron_count_, &ifs) <<
        std::endl;
  }
  ifs.close();
  return os;
}

}  // namespace ann

}  // namespace slesann
