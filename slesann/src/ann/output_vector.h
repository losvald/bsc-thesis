/*
 * output_vector.h
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
 * output_vector.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef OUTPUT_VECTOR_H_
#define OUTPUT_VECTOR_H_

#include "ann_vector.h"

namespace slesann {

namespace ann {

template<typename T>
class OutputVector : public AnnVector<T> {
public:
  OutputVector(std::size_t n) : AnnVector<T>(n) {}
  OutputVector() {}

  static OutputVector Input(std::size_t n, std::istream* is) {
    OutputVector v(n);
    AnnVector<T>::Input(n, &v, is);
    return v;
  }
};

}  // namespace ann

}  // namespace slesann

#endif /* OUTPUT_VECTOR_H_ */
