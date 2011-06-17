/*
 * ann.cpp
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
 * ann.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#include <cassert>

#include <numeric>

#include <fann.h>

#include "ann.h"

namespace slesann {

namespace ann {

Ann::Ann(std::size_t input_neuron_count, std::size_t output_neuron_count,
         const std::vector<std::size_t>& hidden_neuron_counts)
: tmp_input_vector_(input_neuron_count) {
  std::size_t total_count = 2 + hidden_neuron_counts.size();
  unsigned* arr = new unsigned[total_count];
  arr[0] = static_cast<unsigned>(input_neuron_count);
  for (std::size_t i = 0, i_to = hidden_neuron_counts.size(); i < i_to; ++i)
    arr[i + 1] = static_cast<unsigned>(hidden_neuron_counts[i]);
  arr[total_count - 1] = static_cast<unsigned>(output_neuron_count);

  ann_ = fann_create_standard_array(total_count, arr);
  delete[] arr;

  fann_set_activation_steepness_hidden(ann_, 0.1);
  fann_set_activation_steepness_output(ann_, 0.1);

  fann_set_activation_function_hidden(ann_, FANN_SIGMOID_SYMMETRIC);
  fann_set_activation_function_output(ann_, FANN_SIGMOID_SYMMETRIC);

  // TODO
}

Ann::Ann(const boost::filesystem::path& file_path)
: ann_(fann_create_from_file(file_path.string().c_str())) {
}

Ann::~Ann() {
}

void Ann::Run(const InputVector<float>& input_vector,
              OutputVector<float>* output_vector) const {
  tmp_input_vector_ = input_vector;
  float* out = fann_run(ann_, &tmp_input_vector_[0]);
  if (output_vector->size() != output_neuron_count())
    output_vector->resize(output_neuron_count());
  std::copy(out, out + output_neuron_count(), output_vector->begin());
}

std::size_t Ann::input_neuron_count() const {
  return static_cast<std::size_t>(fann_get_num_input(ann_));
}

std::size_t Ann::output_neuron_count() const {
  return static_cast<std::size_t>(fann_get_num_output(ann_));
}

void Ann::Save(const boost::filesystem::path& file_path) const {
  fann_save(ann_, file_path.string().c_str());
}

fann* Ann::ann() const {
  return ann_;
}

void Ann::set_ann(fann* ann) {
  ann_ = ann;
}

}  // namespace ann

}  // namespace slesann
