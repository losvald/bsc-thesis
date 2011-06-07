/*
 * ann_data_transformation.h
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
 * ann_data_transformation.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef ANN_DATA_TRANSFORMATION_H_
#define ANN_DATA_TRANSFORMATION_H_

#include <boost/smart_ptr/shared_ptr.hpp>

#include "input_vector.h"
#include "output_vector.h"

namespace slesann {

namespace ann {

template<typename T, typename U>
class InputDataTransformer {
  std::size_t input_neuron_count_;
public:
  typedef boost::shared_ptr<InputDataTransformer> SharedPtr;

  InputDataTransformer(std::size_t input_neuron_count)
  : input_neuron_count_(input_neuron_count) {}

  virtual InputVector<U> operator()(const T& data) const = 0;

  std::size_t input_neuron_count() const {
    return input_neuron_count_;
  }
};

template<typename T, typename U>
class OutputDataTransformer {
  std::size_t output_neuron_count_;
public:
  typedef boost::shared_ptr<OutputDataTransformer> SharedPtr;

  OutputDataTransformer(std::size_t output_neuron_count)
  : output_neuron_count_(output_neuron_count) {}

  virtual OutputVector<U> operator()(const T& data) const = 0;

  virtual T operator()(const OutputVector<U>& output_vector) const = 0;

  std::size_t output_neuron_count() const {
    return output_neuron_count_;
  }
};

template<typename T>
class DefaultOutputDataTransformer : public OutputDataTransformer<T, T> {
public:
  typedef OutputDataTransformer<T, T> super;

  DefaultOutputDataTransformer() : super(1) {}

  OutputVector<T> operator()(const T& data) const {
    OutputVector<T> ret(1);
    ret[0] = data;
    return ret;
  }

  T operator()(const OutputVector<T>& output_vector) const {
    return output_vector[0];
  }

};

template<typename T>
class SigmoidSymmetricToProbabilityTransformer
: public OutputDataTransformer<T, T> {
public:
  typedef OutputDataTransformer<T, T> super;

  SigmoidSymmetricToProbabilityTransformer() : super(1) {}

  OutputVector<T> operator()(const T& data) const {
    OutputVector<T> ret(1);
    ret[0] = 2 * data - 1;
    return ret;
  }

  T operator()(const OutputVector<T>& output_vector) const {
    return (output_vector[0] + 1) / 2;
  }

};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_DATA_TRANSFORMATION_H_ */
