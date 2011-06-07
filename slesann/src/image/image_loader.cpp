/*
 * image_loader.cpp
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
 * image_loader.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include <opencv/highgui.h>

#include "image_loader.h"

namespace slesann {

//ImageLoader::instance instance;

cv::Mat ImageLoader::LoadImage(const ImagePath& image_path) {
  return cv::imread(image_path.string());
}

ImageLoader ImageLoader::instance() {
  return instance_;
}

}  // namespace slesann
