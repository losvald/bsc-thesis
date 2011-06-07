/*
 * plate_ann.cpp
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
 * plate_ann.cpp
 *
 *  Created on: May 12, 2011
 *      Author: losvald
 */

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "../../base.h"
#include "../transform_utils.h"
#include "plate_ann.h"

#include <cassert>

namespace slesann {

namespace ann {

namespace {

  static const float kResizeFactor = 1 / 8.f;

}

std::size_t VectorSize(const boost::filesystem::path& plate_img_path) {
  cv::Mat img = cv::imread(plate_img_path.string());
  return (img.rows * kResizeFactor) * (img.cols * kResizeFactor);
}

PlateVector PlateToVector(const boost::filesystem::path& p) {
  EZLOGGERSTREAM2(std::cerr) << "Converting to vector: " << p.string() <<
      std::endl;

  cv::Mat img = cv::imread(p.string()), img_grayscale;
  img = Resize(img, kResizeFactor);
  cv::cvtColor(img, img_grayscale, CV_BGR2GRAY);

//  cv::imshow("rgb", img);
//  cv::imshow("grayscale", img_grayscale);
//  cv::waitKey(-1);

  PlateVector ret(0);
  ret.reserve(img_grayscale.rows * img_grayscale.cols);
  for (int r = 0; r < img_grayscale.rows; ++r)
    for (int c = 0; c < img_grayscale.cols; ++c) {
      ret.push_back(img_grayscale.at<uchar>(r, c));
    }

  std::cout << "[" << img_grayscale.cols << " x " << img_grayscale.rows << "]\n";

  EZLOGGERSTREAM2(std::cerr) << "" << p.string() << " -> vector<uchar>(" <<
        ret.size() << std::endl;

  return ret;
}

}  // namespace ann

}  // namespace slesann
