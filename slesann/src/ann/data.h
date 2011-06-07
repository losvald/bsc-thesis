/*
 * data.h
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
 * data.h
 *
 *  Created on: Jun 6, 2011
 *      Author: losvald
 */

#ifndef ANN_DATA_H_
#define ANN_DATA_H_

#include <boost/filesystem/path.hpp>
#include <fann.h>

namespace slesann {

namespace ann {

class AnnData {
public:
  AnnData(const boost::filesystem::path& file_path);
  AnnData(const AnnData& clone);
  ~AnnData();
  void Save(const boost::filesystem::path& file_path) const;
  AnnData& operator=(const AnnData& clone);
  fann_train_data* data() const;
private:
  mutable fann_train_data* data_;
};

}  // namespace ann

}  // namespace slesann

#endif /* ANN_DATA_H_ */
