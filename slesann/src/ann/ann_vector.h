/*
 * ann_vector.h
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
 * ann_vector.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef ANN_VECTOR_H_
#define ANN_VECTOR_H_

#include <iostream>
#include <vector>

namespace slesann {

namespace ann {

template<typename T>
class AnnVector : public std::vector<T> {
public:
  typedef T InputType;

  AnnVector(std::size_t n) : std::vector<T>(n) {}
  AnnVector() {}

  friend std::ostream& operator<<(std::ostream& os, const AnnVector& v) {
    for (std::size_t i = 0, to = v.size(); i < to; ++i) {
      if (i) os << ' ';
      os << v[i];
    }
    return os;
  }

protected:
  static void Input(std::size_t n, AnnVector* v, std::istream* is) {
    v->resize(n);
    for (std::size_t i = 0; i < n; ++i)
       *is >> (*v)[i];
  }
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_VECTOR_H_ */
