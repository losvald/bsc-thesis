/*
 * input_vector.h
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
 * input_vector.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef ANN_INPUT_VECTOR_H_
#define ANN_INPUT_VECTOR_H_

#include <algorithm>

#include "ann_vector.h"

namespace slesann {

namespace ann {

template<typename T>
class InputVector : public AnnVector<T> {
public:
  InputVector(std::size_t n) : AnnVector<T>(n) {}
  InputVector(const std::vector<InputVector<T> >& input_vectors) {
    std::size_t n = 0;
    typedef typename std::vector<InputVector<T> >::const_iterator ConstIter;
    for (ConstIter iv = input_vectors.begin(), end = input_vectors.end();
        iv != end; ++iv)
      n += iv->size();
    this->resize(n);
    std::size_t cur_ind = 0;
    for (ConstIter iv = input_vectors.begin(), end = input_vectors.end();
            iv != end; ++iv) {
      std::copy(iv->begin(), iv->end(), this->begin() + cur_ind);
      cur_ind += iv->size();
    }
  }
  InputVector() {}
public:
  static InputVector Input(std::size_t n, std::istream* is) {
    InputVector v(n);
    AnnVector<T>::Input(n, &v, is);
    return v;
  }
};

}  // namespace slesann

}  // namespace ann

#endif /* ANN_INPUT_VECTOR_H_ */
