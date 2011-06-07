/*
 * image_storage.cpp
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
 * image_storage.cpp
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#include <boost/filesystem.hpp>

#include "image_storage.h"

namespace slesann {

ImageStorage::ImageStorage(const ImagePath& tmp_dir)
: tmp_dir_(tmp_dir) {
  namespace fs = boost::filesystem;
  fs::remove_all(tmp_dir);
  fs::create_directory(tmp_dir);
}

ImageStorage::~ImageStorage() {
  namespace fs = boost::filesystem;
//  fs::remove_all(tmp_dir);
}

const boost::filesystem::path& ImageStorage::tmp_dir_path() const {
  return tmp_dir_;
}

}  // namespace slesann
