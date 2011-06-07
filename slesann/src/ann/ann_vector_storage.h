/*
 * ann_vector_storage.h
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
 * ann_vector_storage.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef ANN_VECTOR_STOGARE_H_
#define ANN_VECTOR_STOGARE_H_

#include <fstream>

#include <boost/filesystem.hpp>

#include "ann_vector.h"
#include "input_vector.h"
#include "output_vector.h"

namespace slesann {

namespace ann {

class AnnVectorStorage {
  const boost::filesystem::path tmp_dir_;
public:
  AnnVectorStorage(const boost::filesystem::path& tmp_dir);
  ~AnnVectorStorage();

  template<typename T>
  void Save(const AnnVector<T>& v, const std::string& name) {
    std::ofstream of;
    of.open(ToPath(name).string().c_str());
    std::cerr << "SAVE TO: " << ToPath(name).string() << std::endl;
    of << v.size() << std::endl;
    of << v << std::endl;
    of.close();
  }

  template<typename T>
  InputVector<T> LoadInputVector(const std::string& name) {
    return Load<InputVector<T> >(name);
  }

  template<typename T>
  OutputVector<T> LoadOutputVector(const std::string& name) {
    return Load<OutputVector<T> >(name);
  }

private:
  boost::filesystem::path ToPath(const std::string& name);

  template<typename T>
  T Load(const std::string& name) {
    std::ifstream ifs;
    ifs.open(ToPath(name).string().c_str());
    std::size_t n; ifs >> n;
    T ret = T::Input(n, &ifs);
    ifs.close();
    return ret;
  }
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_VECTOR_STOGARE_H_ */
