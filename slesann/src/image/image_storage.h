/*
 * image_storage.h
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
 * image_storage.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef IMAGE_STORAGE_H_
#define IMAGE_STORAGE_H_

#include <opencv/cxcore.h>

#include "image_base.h"

namespace slesann {

class ImageStorage {
  const ImagePath tmp_dir_;
public:
  ImageStorage(const ImagePath& tmp_dir);
  ~ImageStorage();

  const ImagePath& tmp_dir_path() const;
};

}  // namespace slesann

#endif /* IMAGE_STORAGE_H_ */
