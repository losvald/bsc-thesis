/*
 * boundary_detector.h
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
 * boundary_detector.h
 *
 *  Created on: May 18, 2011
 *      Author: losvald
 */

#ifndef BOUNDARY_DETECTOR_H_
#define BOUNDARY_DETECTOR_H_

#include "image_base.h"
#include "pixel_utils.h"

namespace slesann {

template<int colorSpace>
class EmptyColorBoundaryDetector {
public:
  EmptyColorBoundaryDetector(const cv::Mat& img) : img_(img) {
//    std::cerrr << "EmptyColorBOundaryDetector " << img.cols << " x "
//        << img.rows << "\n";
  }

  bool operator()(int x, int y) const {
    if (kEmptyColor.IsEmpty<colorSpace>(img_.at<cv::Vec3b>(y, x)))
      return false;

    PixelPosition pp(x, y);
    if (!IsValid(pp))
        return false;
    for (std::size_t i = 0; i < 8; ++i) {
      const PixelPosition& neigh = pp.neighbor(i);
      if (IsValid(neigh) && kEmptyColor.IsEmpty<colorSpace>(
          img_.at<cv::Vec3b>(neigh.y(), neigh.x())))
        return true;
    }
    return false;
  }
private:
  cv::Mat img_;

  bool IsValid(const PixelPosition& pp) const {
    return pp.x() >= 0 && pp.x() < img_.cols &&
        pp.y() >= 0 && pp.y() < img_.rows;
  }
};

}  // namespace slesann

#endif /* BOUNDARY_DETECTOR_H_ */
