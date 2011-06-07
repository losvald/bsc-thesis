/*
 * ann.h
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
 * ann.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef ANN_H_
#define ANN_H_

#include <vector>

#include <boost/filesystem/path.hpp>
#include <fann.h>

#include "input_vector.h"
#include "output_vector.h"

namespace slesann {

namespace ann {

class Ann {
public:
  Ann(std::size_t input_neuron_count, std::size_t output_neuron_count,
      const std::vector<std::size_t>& hidden_neuron_counts);
  Ann(const boost::filesystem::path& file_path);
  ~Ann();
  void Run(const InputVector<float>& input_vector,
           OutputVector<float>* output_vector) const;
  void Save(const boost::filesystem::path& file_path) const;

  std::size_t input_neuron_count() const;
  std::size_t output_neuron_count() const;
  fann* ann() const;
  void set_ann(fann* ann);
private:
  mutable struct fann* ann_;
  mutable std::vector<float> tmp_input_vector_;
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_H_ */
