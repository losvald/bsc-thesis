/*
 * match_display.cpp
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
 * match_display.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#ifndef MATCH_DISPLAY_CPP_
#define MATCH_DISPLAY_CPP_

#include "../../src/image/image_loader.h"
#include "../../src/image/transform_utils.h"
#include "match_display.h"

#include <boost/filesystem.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

namespace slesann {

namespace {

void Copy(const cv::Mat& src, int x_dst, int y_dst, cv::Mat* dst) {
  int old_x_dst = x_dst;
  for (int y_src = 0; y_src < src.rows; ++y_src, ++y_dst) {
    x_dst = old_x_dst;
    for (int x_src = 0; x_src < src.cols; ++x_src, ++x_dst)
      dst->at<cv::Vec3b>(y_dst, x_dst) = src.at<cv::Vec3b>(y_src, x_src);
  }
}

}  // namespace

void ShowMatchingImages(const ImagePath& img_path1, const ImagePath& img_path2,
                        float scale_factor,
                        bool wait_key) {
  cv::Mat img1 = Resize(ImageLoader::instance().LoadImage(img_path1),
                        scale_factor);
  cv::Mat img2 = Resize(ImageLoader::instance().LoadImage(img_path2),
                        scale_factor);

  cv::Size img1_size = img1.size();
  cv::Mat img(std::max(img1.rows, img2.rows),
              2 * std::max(img1.cols, img2.cols),
              CV_8UC3,
              cv::Scalar(cv::Vec3b(0, 0, 0)));

  Copy(img1, (img.cols / 2 - img1.cols) / 2, (img.rows - img1.rows) / 2, &img);
  Copy(img2, img.cols / 2 + (img.cols / 2 - img2.cols) / 2,
       (img.rows - img2.rows) / 2, &img);

  std::string win_name = boost::filesystem::basename(img_path1.filename()) +
      " <-> " + boost::filesystem::basename(img_path2.filename());
  cv::imshow(win_name, img);

  if (wait_key)
    cv::waitKey(-1);
}

}  // namespace slesann

#endif /* MATCH_DISPLAY_CPP_ */
