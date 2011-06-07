/*
 * shape_discrimination_test.cpp
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
 * shape_discrimination_test.cpp
 *
 *  Created on: May 26, 2011
 *      Author: losvald
 */

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "shape_discrimination_test.h"
#include "boundary_test.h"
#include "../statistics.h"
#include "../../src/image/image_base.h"
#include "../../src/image/image_loader.h"
#include "../../src/image/boundary_detector.h"
#include "../../src/image/transform_utils.h"
#include "../../src/image/geometry_utils.h"
#include "../../src/image/shape_discriminator.h"

namespace slesann {

namespace test {

namespace {



//template<class TPixelPositionPredicate>
//class HullPixelFilter {
//public:
//  std::vector<PixelPosition> operator()(const Hull& h,
//                                        const TPixelPositionPredicate& f) const {
//    std::vector<PixelPosition> ret;
//    BOOST_FOREACH(const PixelPosition& pp, h.pixel_positions()) {
//      if (f(pp))
//        ret.push_back(pp);
//    }
//    return ret;
//  }
//};

//class SideFilter {
//private:
//  PixelPosition bb_center_;
//public:
//  SideFilter(const Hull& h) : bb_center_(h.bounding_box().center()) {}
//  const PixelPosition& bounding_box_center() const;
//};
//
//class LeftSideFilter : public SideFilter {
//  LeftSideFilter(const Hull& h) : SideFilter(h) {}
//  bool operator()(const PixelPosition& pp) const {
//    return pp.x() <= bounding_box_center().x();
//  }
//};
//
//class RightSideFilter : public SideFilter {
//  RightSideFilter(const Hull& h) : SideFilter(h) {}
//  bool operator()(const PixelPosition& pp) const {
//    return pp.x() >= bounding_box_center().x();
//  }
//};


const std::string kWhRatio = "width / height";
const std::string kNormalizedHeight = "normalized height";
const std::string kHeightPerimeterQuotient = "height / perimeter";
const std::string kWidthPerimeterQuotient = "width / perimeter";
const std::string kPerimeterQuotient = "perimeter quotient";
const std::string kRectangularity = "rectangularity";
const std::string kSideAvg = "side function average";
const std::string kSideMedian = "side function median";

const std::string kProperties[] = {kWhRatio, kPerimeterQuotient, kRectangularity,
                                   kSideAvg, kSideMedian};

}  // namespace

void ComputeShapeDiscriminationStatistics(
    const boost::filesystem::path& file_path) {
  std::fstream ifs;
  ifs.open(file_path.string().c_str());

  MeasurementMap mm_cur, mm_all, mm_group_avg, mm_group_stddev,
    mm_group_relerr, mm_group_maxdiff;

  for (std::string s; ifs >> s; ) {
    if (s[0] == '-') {
      PrintStatistics(mm_cur, "Group average");
      std::cout << std::endl;
      DoAfterGroup(&mm_cur, &mm_all, &mm_group_avg, &mm_group_stddev,
                   &mm_group_relerr, &mm_group_maxdiff);
      continue;
    }

    ImagePath img_path(s);
//    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.2);
    cv::Mat img = ShapeDiscriminator::LoadImage(img_path);

    Hull h = FindBoundary(img);

    mm_cur[kWhRatio].Add(ShapeDiscriminator::ComputeWidthHeightRatio(h));
    mm_cur[kNormalizedHeight].Add(ShapeDiscriminator::GetNormalizedHeight(h));
    mm_cur[kHeightPerimeterQuotient].Add(ShapeDiscriminator::GetHeightPerimeterQuotient(h));
    mm_cur[kWidthPerimeterQuotient].Add(ShapeDiscriminator::GetWidthPerimeterQuotient(h));
    mm_cur[kPerimeterQuotient].Add(ShapeDiscriminator::GetPerimeterQuotient(h));
    mm_cur[kRectangularity].Add(ShapeDiscriminator::GetRectangularity(h));
    mm_cur[kSideMedian].Add(ShapeDiscriminator::GetSideFunctionMedian(h));
    mm_cur[kSideAvg].Add(ShapeDiscriminator::GetSideFunctionAverage(h));

    std::cout << "image: " << img_path.string() << std::endl;
//    std::cout << "circumference:  " << h.circumference() << std::endl;
//    std::cout << "area:           " << h.convex_area() << std::endl;
    PrintLastMeasurements(mm_cur);

    std::cout << std::endl;

//    cv::imshow(img_path.string(), img);
//    cv::imshow("shape-detection-test", img);

    cv::Mat framed_img = SetBoundaryFrame(img);
//    cv::imshow(img_path.string() + "-boundary", framed_img);
//    cv::imshow("shape-detection-test", framed_img);
//    cv::waitKey(-1);
  }
  ifs.close();

  PrintStatistics(mm_all, "Global statistics");
  std::cout << std::endl;
  PrintStatistics(mm_group_avg, "Group Average");
  std::cout << std::endl;
  PrintStatistics(mm_group_stddev, "Group std. dev.");
  std::cout << std::endl;
  PrintStatistics(mm_group_relerr, "Group Relative Error");
  std::cout << std::endl;
  PrintStatistics(mm_group_maxdiff, "Group max. diff.");
  std::cout << std::endl;
  PrintUniqueness(mm_group_stddev, mm_group_avg);
}

}  // namespace test

}  // namespace slesann
