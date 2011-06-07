/*
 * plate_input_transformer.cpp
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
 * plate_input_transformer.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include "plate_input_transformer.h"

#include <opencv/highgui.h>

namespace slesann {

namespace ann {

namespace {

InputVector<float> ComputePlateHash(const cv::Mat& img) {
  // TODO
}

}  // namespace

PlateInputTransformer::PlateInputTransformer()
: ImageInputTransformer(0) { // FIXME
}

InputVector<float> PlateInputTransformer::operator()(
    const ImagePath& image_path) const {
  return ComputePlateHash(cv::imread(image_path.string()));
}

}  // namespace ann

}  // namespace slesann
