/*
 * boundary_test.h
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
 * boundary_test.h
 *
 *  Created on: May 19, 2011
 *      Author: losvald
 */

#ifndef BOUNDARY_TEST_H_
#define BOUNDARY_TEST_H_

#include <boost/filesystem/path.hpp>

#include <opencv/cxcore.h>

#include "../../src/image/pixel_utils.h"

namespace slesann {

namespace test {

Hull FindBoundary(const cv::Mat& img);

cv::Mat SetBoundaryFrame(const cv::Mat& img);

void FindAndShowGreatestHull(const boost::filesystem::path& file_path);

}  // namespace test

}  // namespace slesann

#endif /* BOUNDARY_TEST_H_ */
