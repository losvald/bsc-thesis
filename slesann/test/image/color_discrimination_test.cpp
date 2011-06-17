/*
 * color_discrimination_test.cpp
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
 * color_discrimination_test.cpp
 *
 *  Created on: May 26, 2011
 *      Author: losvald
 */

#include <iostream>
#include <iomanip>
#include <fstream>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "color_discrimination_test.h"
#include "../statistics.h"
#include "../../src/image/boundary_detector.h"
#include "../../src/image/color_discriminator.h"
#include "../../src/image/geometry_utils.h"
#include "../../src/image/image_base.h"
#include "../../src/image/image_loader.h"
#include "../../src/image/transform_utils.h"

namespace slesann {

namespace test {

namespace {

void PrintGreenHsv(const cv::Mat& img) {
  std::cout << "Green hsv:";
  for (int r = 0; r < 10; ++r)
    for (int c = 0; c < 10; ++c) {
      cv::Vec3b px = img.at<cv::Vec3b>(r, c);
      std::cout << (float)px[0] << ' ' << (float)px[1] << ' '
          << (float)px[2] << std::endl;
    }
}

void PaintPink(cv::Mat& img) {
  const cv::Vec3b kPink(122, 0, 255);
  for (int r = 0; r < 7; ++r)
    for (int c = 0; c < 7; ++c) {
      img.at<cv::Vec3b>(r, c) = kPink;
    }
}

void DrawHorizontalLine(cv::Mat& img, const Hull& h, float f) {
  const Rectangle& bb = h.bounding_box();
  int line_y = static_cast<int>(bb.first.y() + f * bb.height());
  const cv::Vec3b kMagenta(0xFF, 0, 0xFF);
  for (int x = 0; x < img.cols; ++x)
    img.at<cv::Vec3b>(line_y, x) = kMagenta;
}

void PaintInside(cv::Mat& img, const cv::Mat& img_hsv, const Hull& h) {
  const cv::Vec3b kBlue(0xFF, 0, 0);
  const cv::Vec3b kRed(0, 0, 0xFF);
  const Rectangle& bb = h.bounding_box();
  int y_windshield_begin = static_cast<int>(bb.first.y() + 0.05 * bb.height());
  int y_windshield_end = static_cast<int>(bb.first.y() + 0.3 * bb.height());
  int y_bumper_begin = static_cast<int>(bb.first.y() + 0.58 * bb.height());
  for (int r = 0; r < img_hsv.rows; ++r) {
    if (r >= y_windshield_begin && r <= y_windshield_end || r >= y_bumper_begin)
      continue;
    for (int c = 0; c < img.cols; ++c) {
      cv::Vec3b& px = img.at<cv::Vec3b>(r, c);
      const cv::Vec3b& px_hsv = img_hsv.at<cv::Vec3b>(r, c);
      if (!kEmptyColor.IsEmpty<kHsv>(px_hsv) &&
          h.GreedyInside(PixelPosition(c, r))) {
        if (ColorDiscriminator::IsHueDominant(px_hsv[1], px_hsv[2])) {
          px = kRed;
        } else {
          px = kBlue;
        }
      }
    }
  }
}

template<int ColorSpace>
void ShowColor(const cv::Vec3b& color,
               const std::string& win_name = "color") {
  const int kSize = 100;
  cv::Mat img(kSize, kSize, CV_8UC3);
  for (int r = 0; r < img.rows; ++r)
    for (int c = 0; c < img.cols; ++c)
      img.at<cv::Vec3b>(r, c) = color;

  cv::Mat img_bgr;
  switch (ColorSpace) {
  case kHsv:
    cv::cvtColor(img, img_bgr, CV_HSV2BGR);
    break;
  default:
    img_bgr = img;
  }
  cv::imshow(win_name, img_bgr);
}

//cv::Vec3b GetDominantColor(const ColorDiscriminator::HueDominator& hd,
//                           const ColorDiscriminator::IntensityDominator& id) {
//  return hd.dominance() > 0 ? cv::Vec3b(hd.
//}

const std::string kDominatingColor = "dominating color";
const std::string kDominatingHueLsf = "dom. hue least square fit";
const std::string kDominatingIntensityMedian = "dom. intensity median";
const std::string kHueLeastSquareFit = "hue least square fit";
const std::string kSaturationAverage = "saturation average";
const std::string kIntensityAverage = "intensity average";
const std::string kHueDominance = "hue dominance";

const std::string kProperties[] = {//kDominatingColor,
//                                   kDominatingHueLsf,
//                                   kDominatingIntensityMedian,
                                   kHueLeastSquareFit,
                                   kSaturationAverage,
                                   kIntensityAverage,
                                   kHueDominance};

} // namespace

void RunColorDetection(const boost::filesystem::path& file_path) {
  std::fstream ifs;
  ifs.open(file_path.string().c_str());
  for (std::string s; ifs >> s; ) {
    if (s[0] == '-')
      continue;

    ImagePath img_path(s);
//    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.10);
    cv::Mat img = ColorDiscriminator::LoadImage(img_path);
    Hull h = FindGreatestHull(img, EmptyColorBoundaryDetector<kBgr>(img));

    cv::Mat img_hsv; cv::cvtColor(img, img_hsv, CV_BGR2HSV);

    DrawHorizontalLine(img, h, 0.58);

    cv::imshow("img-2", img);

    ColorDiscriminator::HueDominator hue_dominator;
    ColorDiscriminator::IntensityDominator intensity_dominator;
    uchar hue_lsf, saturation_avg, intensity_avg;
    std::vector<PixelPosition> body_pixels;
    ColorDiscriminator::GetBodyPixels(img_hsv, h, &body_pixels);

    ColorDiscriminator::ComputeDominators(img_hsv, body_pixels,
                                          &hue_dominator,
                                          &intensity_dominator,
                                          &hue_lsf, &saturation_avg,
                                          &intensity_avg);

    std::cout << "image: " << img_path.string() << std::endl;
    std::cout << "dom. hue: " << hue_dominator.hue_str() << '(' <<
        hue_dominator.dominance_str() << ")\n";
    std::cout << "dom. int: " << intensity_dominator.intensity_str() << '(' <<
        intensity_dominator.dominance_str() << ")\n";
    std::cout << "lsf hue: " << HueToString(hue_lsf) << std::endl;

    cv::Vec3b bgr_avg = ColorDiscriminator::GetAverageBgrColor(img, body_pixels);
    std::cout << "RGB average: " << ToRgbString(bgr_avg) << std::endl;

    cv::Vec3b hsv_avg = BgrToHsv(bgr_avg);
    std::cout << "HSV average: " << ToHsvString(hsv_avg) << std::endl;

    PaintInside(img, img_hsv, h);
    cv::imshow("img", img);
    ShowColor<kHsv>(cv::Vec3b(hue_lsf, saturation_avg, intensity_avg),
                    "hue-lsf, sat-avg, int-avg");

    ShowColor<kHsv>(cv::Vec3b(hue_dominator.hue(), saturation_avg, intensity_avg),
                    "hue-dom, sat-avg, int-avg");

    ShowColor<kBgr>(bgr_avg, "RGB average");
    cv::waitKey(-1);

//    PrintGreenHsv(img_hsv);
  }
}

void ComputeColorDiscriminationStatistics(
    const boost::filesystem::path& file_path) {
  MeasurementMap mm_cur, mm_all, mm_group_avg, mm_group_stddev,
       mm_group_relerr, mm_group_maxdiff;

  std::fstream ifs;
  ifs.open(file_path.string().c_str());
  for (std::string s; ifs >> s; ) {
    if (s[0] == '-') {
      PrintStatistics(mm_cur, "Inside-group average");
      std::cout << std::endl;
      DoAfterGroup(&mm_cur, &mm_all, &mm_group_avg, &mm_group_stddev,
                   &mm_group_relerr, &mm_group_maxdiff);
      continue;
    }

    ImagePath img_path(s);
    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.10);
    Hull h = FindGreatestHull(img, EmptyColorBoundaryDetector<kBgr>(img));

    cv::Mat img_hsv; cv::cvtColor(img, img_hsv, CV_BGR2HSV);

    ColorDiscriminator::HueDominator hue_dominator;
    ColorDiscriminator::IntensityDominator intensity_dominator;
    uchar hue_lsf, saturation_avg, intensity_avg;
    std::vector<PixelPosition> body_pixels;
    ColorDiscriminator::GetBodyPixels(img_hsv, h, &body_pixels);

    ColorDiscriminator::ComputeDominators(img_hsv, body_pixels,
                                          &hue_dominator,
                                          &intensity_dominator,
                                          &hue_lsf, &saturation_avg,
                                          &intensity_avg);

    cv::Vec3b bgr_avg = ColorDiscriminator::GetAverageBgrColor(img, body_pixels);
    cv::Vec3b hsv_avg = BgrToHsv(bgr_avg);

    if (hue_dominator.dominance() > intensity_dominator.dominance()) {
      // TODO
    }

    std::cout << "image: " << img_path.string() << std::endl;

    mm_cur[kHueLeastSquareFit].Add(2 * hue_lsf);
    mm_cur[kSaturationAverage].Add(saturation_avg);
    mm_cur[kIntensityAverage].Add(intensity_avg);
    mm_cur[kHueDominance].Add(hue_dominator.dominance());
    mm_cur["avg hue"].Add(hsv_avg[0] * 2);
    mm_cur["avg saturation"].Add(hsv_avg[1] / 2.55f);
    mm_cur["avg intensity"].Add(hsv_avg[2] / 2.55f);

    PrintLastMeasurements(mm_cur);
    std::cout << std::endl;
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

