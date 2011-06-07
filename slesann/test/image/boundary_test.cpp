/*
 * boundary_test.cpp
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
 * boundary_test.cpp
 *
 *  Created on: May 19, 2011
 *      Author: losvald
 */

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "boundary_test.h"
#include "../statistics.h"
#include "../../src/image/image_base.h"
#include "../../src/image/image_loader.h"
#include "../../src/image/boundary_detector.h"
#include "../../src/image/transform_utils.h"
#include "../../src/image/geometry_utils.h"

namespace slesann {

namespace test {

namespace {

std::vector<PixelPosition> FindBoundaryBf(const cv::Mat& img) {
  std::vector<PixelPosition> ret;
  EmptyColorBoundaryDetector<kBgr> d(img);
  for (int y = 0; y < img.rows; ++y)
    for (int x = 0; x < img.cols; ++x) {
      if (d(x, y))
        ret.push_back(PixelPosition(x, y));
    }
  return ret;
}

std::vector<PixelPosition> FindBoundarySemiBf(const cv::Mat& img) {
  std::vector<PixelPosition> ret;
  EmptyColorBoundaryDetector<kBgr> d(img);
  for (int y = 0; y < img.rows; ++y)
    for (int x = 0; x < img.cols; ++x) {
      if (d(x, y))
        return HullBfs(PixelPosition(x, y), d).pixel_positions();
    }
  return std::vector<PixelPosition>();
}

}  // namespace

Hull FindBoundary(const cv::Mat& img) {
  EmptyColorBoundaryDetector<kBgr> d(img);
  return FindGreatestHull(img, d);
}

Hull FindBoundaryHsv(const cv::Mat& img) {
  cv::Mat img_hsv; cv::cvtColor(img, img_hsv, CV_BGR2HSV);
  return FindGreatestHull(img_hsv, EmptyColorBoundaryDetector<kHsv>(img_hsv));
}

Hull FindBoundaryHsv2(const cv::Mat& img_hsv) {
  return FindGreatestHull(img_hsv, EmptyColorBoundaryDetector<kHsv>(img_hsv));
}

cv::Mat SetBoundaryFrame(const cv::Mat& img) {
  const cv::Vec3b kMagenta(0xFF, 0, 0xFF);
  cv::Mat ret = img;
  cv::Mat img_hsv; cv::cvtColor(img, img_hsv, CV_BGR2HSV);
  // TODO FindBoundary(img).pixel_positions() inside BOOST_FOREACH doesn't work
  const Hull h = FindBoundary(img);
  BOOST_FOREACH(const PixelPosition& pp, h.pixel_positions()) {
    //std::cerr << "Boundary point: " << pp << std::endl;
    if (pp.y() >= 0 && pp.y() < ret.rows && pp.x() >= 0 && pp.x() < ret.cols)
      ret.at<cv::Vec3b>(pp.y(), pp.x()) = kMagenta;
    else {
      std::cerr << pp << " outside " << PixelPosition(ret.cols, ret.rows) << "\n";
    }
  }
  return ret;
}

void FindAndShowGreatestHull(const boost::filesystem::path& file_path) {
  std::fstream ifs;
  ifs.open(file_path.string().c_str());

  for (std::string s; ifs >> s; ) {
    if (s[0] == '-')
      continue;

    ImagePath img_path(s);
    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.2);
    cv::Mat framed_img = SetBoundaryFrame(img);
    cv::imshow("greatest-hull", framed_img);
    cv::waitKey(-1);
  }
  ifs.close();
}

}  // namespace test

}  // namespace slesann
