/*
 * plate_ann.h
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
 * plate_ann.h
 *
 *  Created on: May 12, 2011
 *      Author: losvald
 */

#ifndef PLATE_ANN_H_
#define PLATE_ANN_H_

#include <vector>

#include <boost/filesystem.hpp>

#include "../../ann/input_vector.h"

namespace slesann {

namespace ann {

typedef short PlateVectorElement;
typedef InputVector<PlateVectorElement> PlateVector;

std::size_t VectorSize(const boost::filesystem::path& plate_img_path);

PlateVector PlateToVector(const boost::filesystem::path& p);

}  // namespace ann

}  // namespace slesann

#endif /* PLATE_ANN_H_ */
