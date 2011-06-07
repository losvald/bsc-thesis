/*
 * image_loader.h
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
 * image_loader.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef IMAGE_LOADER_H_
#define IMAGE_LOADER_H_

#include "image_base.h"

namespace slesann {

class ImageLoader {
public:
  cv::Mat LoadImage(const ImagePath& image_path);
  static ImageLoader instance();
private:
  static ImageLoader instance_;
};

}  // namespace slesann

#endif /* IMAGE_LOADER_H_ */
