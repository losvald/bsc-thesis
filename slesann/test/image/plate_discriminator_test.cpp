/*
 * plate_discriminator_test.cpp
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
 * plate_discriminator_test.cpp
 *
 *  Created on: May 30, 2011
 *      Author: losvald
 */

#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <opencv/highgui.h>
#include <opencv/cv.h>

#include "../statistics.h"
#include "../../src/image/binary_image_utils.h"
#include "../../src/image/image_base.h"
#include "../../src/image/image_loader.h"
#include "../../src/image/boundary_detector.h"
#include "../../src/image/component_utils.h"
#include "../../src/image/geometry_utils.h"
#include "../../src/image/plate_discriminator.h"
#include "../../src/image/transform_utils.h"
#include "../../src/util/string_utils.h"
#include "plate_discriminator_test.h"

#include "../../src/util/sparse_multiset.h"


namespace slesann {

namespace test {

namespace {

const std::string kBlackWhiteRatio = "black/white ratio";
const std::string kCenterBlackWhiteRatio = "center black/white ratio";
const std::string kNormCentroidX = "norm. centroid x";
const std::string kNormCentroidY = "norm. centroid y";


std::string GetNthCentroidName(std::size_t n, char x_or_y) {
  return MakeString() << "norm. centroid #" << n << ' ' << x_or_y;
}


cv::Mat MyRemoveThinRegions(const cv::Mat& img_binary,
                            int block_size, double c,
                            int method = CV_ADAPTIVE_THRESH_GAUSSIAN_C) {
  cv::Mat img_thres;
  cv::adaptiveThreshold(img_binary, img_thres, 0xFF,
                        method, CV_THRESH_BINARY,
                        block_size, c);
  return img_thres;
}


void TestRemoveThinRegions4(const cv::Mat& img_binary) {
  cv::imshow("binary image", img_binary);
  std::cout << "Image height: " << img_binary.rows << std::endl;
  const ThinRegionRemover& rem = PlateDiscriminator::NonCharacterRegionsRemover();

  const cv::Mat all_white(img_binary.rows, img_binary.cols, CV_8U,
                          cv::Scalar(0xFF));
  cv::Mat cur_img = img_binary;
  for (int itr = 0; itr <= 4; ++itr) {
    cv::Mat img_filtered = rem(img_binary, itr);

    std::cout << "original - min" << std::endl;
    cv::Mat img_filtered_compl;
    cv::subtract(all_white, img_filtered, img_filtered_compl);
    cv::max(img_filtered_compl, cur_img, cur_img);
    cv::imshow(MakeString() << "trr itr #" << itr, cur_img);
    cv::waitKey(-1);
  }
}

void TestRemoveThinRegions3(const cv::Mat& img_binary) {
  cv::imshow("binary image", img_binary);
  std::cout << "Image height: " << img_binary.rows << std::endl;
  typedef std::pair<int, double> BsC;
  const BsC bs_cs[] = {
                       // all (intensively tested)

                       BsC(3, 100),
                       BsC(5, 116),
                       BsC(7, 120), //  125 for < 0.3
                       BsC(9, 128), // 127; 150 for < 0.3
                       BsC(11, 130), // no longer good for small
                       BsC(13, 134), // 130 for big
                       BsC(15, 136), // 132 for big
                       BsC(17, 148), // no longer good for medium
                       BsC(19, 154), // 138 for big



//                       BsC(3,115), BsC(3,120),
//                       BsC(5,120),
//                       BsC(7, 140), //130?
//                       BsC(9,155), // 150?
//                       BsC(11,175)

                       // 1.0
//                       BsC(15,118), //BsC(19,120),
//                       BsC(25,135), //BsC(31, 140),
//                       BsC(35, 150), //BsC(39,155),
//                       BsC(51,175)

  };

  int cur_bs = 21;

//  cv::Mat img_ma
  cv::Mat min(img_binary.rows, img_binary.cols, CV_8U, cv::Scalar(0xFF));
  int last_bs = -1;
  BOOST_FOREACH(const BsC& bs_c, bs_cs) {
    int block_size = bs_c.first; double c = bs_c.second;
    std::cout << "bs = " << block_size << "; c = " << c << std::endl;
    cv::Mat img = MyRemoveThinRegions(img_binary, block_size, c,
                                    CV_ADAPTIVE_THRESH_GAUSSIAN_C);
    cv::imshow(MakeString() << "bs = " << block_size << "; c = " << c,
               img);

    if (last_bs < cur_bs && block_size == cur_bs) {
      cv::imshow("min{all}", min);
      cv::waitKey(-1);
    }
    cv::min(img, min, min);
    if (block_size == cur_bs) {
      cv::imshow("min{all}", min);
      cv::waitKey(-1);
    }
    last_bs = block_size;
  }
  cv::imshow("min{all}", min);
  cv::waitKey(-1);
}

void TestRemoveThinRegions2(const cv::Mat& img_binary) {
  cv::imshow("binary image", img_binary);
  std::cout << "Image height: " << img_binary.rows << std::endl;
  while (true) {
    std::cout << "Input block size and c, focus window and press a key" <<
        std::endl;
    int block_size; double c;
    cv::waitKey(-1);
    std::cin >> block_size >> c;
    if (block_size % 2 == 0) break;
    cv::imshow(MakeString() << "bs = " << block_size << "; c = " << c,
               MyRemoveThinRegions(img_binary, block_size, c));
  }
}

void TestRemoveThinRegions(const cv::Mat& img_binary) {
  cv::imshow("binary image", img_binary);
  std::cout << "Image height: " << img_binary.rows << std::endl;
  const int block_sizes[] = {5, 7, 9, 11, 13, 15, 17};
  for (double c = 120; c <= 200; c += 10) {
    if (abs(c) < 120) continue;
    std::cout << "c = " << c << std::endl;
    BOOST_FOREACH(int block_size, block_sizes) {
      const int methods[] = {//CV_ADAPTIVE_THRESH_MEAN_C,
                             CV_ADAPTIVE_THRESH_GAUSSIAN_C};
      BOOST_FOREACH(int method, methods) {
        cv::Mat img = MyRemoveThinRegions(img_binary, block_size, c, method);
        cv::imshow(MakeString() << "bs = " << block_size << " " <<
                   (method == CV_ADAPTIVE_THRESH_MEAN_C ? "mean" : "gauss"),
                   img);
      }
    }
    cv::waitKey(-1);
  }
}

void TestAdaptiveThresholdMean(const cv::Mat& eq_hist) {
  std::cout << "rows = " << eq_hist.rows << std::endl;
//    int block_sizes[] = {55, 67, 81};
  for (int c = 35; c <= 55; c += 4) {
    std::cout << "c = " << c << std::endl;
    std::cout << "c2= " << c << std::endl;
    //BOOST_FOREACH(int block_size, block_sizes) {
    cv::Mat img_thres = PlateDiscriminator::AdaptiveThresholdMean(eq_hist, c);
    cv::imshow(MakeString() << "threshold-mean ", img_thres);
    cv::Mat img_thres_small = PlateDiscriminator::AdaptiveThresholdMean(
        Resize(eq_hist, 0.5), c);
    cv::imshow(MakeString() << "small threshold-mean ", img_thres_small);
    //}
    cv::Mat img_fixed_thres;
    double threshold = 122;
    cv::threshold(eq_hist, img_fixed_thres, threshold, 0xFF, CV_THRESH_BINARY);
    cv::imshow(MakeString() << "threshold-fixed " << threshold, img_fixed_thres);

    cv::waitKey(-1);
  }
}

double GetBlackWhiteRatio(const cv::Mat& img_binary) {
//  int y_from = static_cast<int>(img_binary.rows *
//                                PlateDiscriminator::kNormCharBeginY);
//  int y_to = static_cast<int>(img_binary.rows *
//                              PlateDiscriminator::kNormCharEndY);
  int y_from = 0, y_to = img_binary.rows;
  return static_cast<double>(
      GetBlackPixelCount(img_binary, y_from, y_to)) /
      img_binary.rows / img_binary.cols;
}

double GetCenterBlackWhiteRatio(const cv::Mat& img_binary) {
  static const float kHeightFactor = 0.20;
  int h = img_binary.rows;
  int y_from = static_cast<int>(h / 2 * (1 - kHeightFactor));
  int y_to = h - y_from;
  return static_cast<double>(
      GetBlackPixelCount(img_binary, y_from, y_to)) /
      img_binary.rows / img_binary.cols;
}

std::pair<float, float> GetNormalizedCentroid(const cv::Mat& img_binary,
                                              const Rectangle& rect) {
  PixelPosition centroid;
  int black_pixel_cnt = 0;
  for (int y = rect.first.y(); y < rect.second.y(); ++y)
    for (int x = rect.first.x(); x < rect.second.x(); ++x)
      if (img_binary.at<uchar>(y, x)) {
        centroid += PixelPosition(x, y);
        ++black_pixel_cnt;
      }
  if (black_pixel_cnt == 0)
    return std::make_pair(.5f, .5f);
  centroid /= black_pixel_cnt;
  return centroid.Normalized<float>(rect.first, rect.second);
}

std::vector<std::pair<float, float> > GetNormalizedCentroids(
    const cv::Mat& img_binary, int n) {
  int width = img_binary.cols;
  Rectangle rect(0,
                 PlateDiscriminator::kNormCharBeginY * img_binary.rows,
                 img_binary.cols,
                 PlateDiscriminator::kNormCharEndY * img_binary.rows);
  std::vector<std::pair<float, float> > centroids;
  for (int i = 0; i < n; ++i) {
    rect.first.x() = i * width / n;
    rect.second.x() = std::min((i + 1) * width / n, width);
//    std::cout << "Rect: " << rect << std::endl;
    centroids.push_back(GetNormalizedCentroid(img_binary, rect));
//    std::cerr << "Centroid #" << i << ": (" << centroids.back().first <<
//        ", " << centroids.back().second << ")\n";
  }
  return centroids;
}

std::string GetMomentName(std::size_t n) {
  static const char kMomentNames[][5] = {"m00",
                                         "m01",
                                         "m02",
                                         "m03",
                                         "m10",
                                         "m11",
                                         "m12",
                                         "m20",
                                         "m21",
                                         "m30",
                                         "mu02",
                                         "mu03",
                                         "mu11",
                                         "mu12",
                                         "mu20",
                                         "mu21",
                                         "mu30",
                                         "nu02",
                                         "nu03",
                                         "nu11",
                                         "nu12",
                                         "nu12",
                                         "nu20",
                                         "nu21",
                                         "nu30"};
  return kMomentNames[n];
}

std::vector<double> GetMoments(const cv::Mat& img_binary) {
  cv::Moments m = cv::moments(img_binary, true);
  std::vector<double> ret;
  ret.push_back(m.m00);
  ret.push_back(m.m01);
  ret.push_back(m.m02);
  ret.push_back(m.m03);
  ret.push_back(m.m10);
  ret.push_back(m.m11);
  ret.push_back(m.m12);
  ret.push_back(m.m20);
  ret.push_back(m.m21);
  ret.push_back(m.m30);
  ret.push_back(m.mu02);
  ret.push_back(m.mu03);
  ret.push_back(m.mu11);
  ret.push_back(m.mu12);
  ret.push_back(m.mu20);
  ret.push_back(m.mu21);
  ret.push_back(m.mu30);
  ret.push_back(m.nu02);
  ret.push_back(m.nu03);
  ret.push_back(m.nu11);
  ret.push_back(m.nu12);
  ret.push_back(m.nu12);
  ret.push_back(m.nu20);
  ret.push_back(m.nu21);
  ret.push_back(m.nu30);
  return ret;

}

//std::pair<int, int> GetCharacterYRegion() {
//  const kMinBlackRatio
//}

void ShowPixelStatistics(const cv::Mat& img_binary) {
  SparseMultiset<uchar, 0x100> s;
  for (int y = 0; y < img_binary.rows; ++y)
    for (int x = 0; x < img_binary.cols; ++x)
      s.Add(img_binary.at<uchar>(y, x));

  std::cout << "Pixel stats" << std::endl;
  std::cout << s << std::endl;
}

void DrawHorizontalLine(cv::Mat& img, float f,
                        cv::Range x_range = cv::Range::all()) {
  int line_y = static_cast<int>(f * img.rows);
  const cv::Vec3b kMagenta(0xFF, 0, 0xFF);
  int x_from = (x_range == cv::Range::all() ? 0 : x_range.start);
  int x_to = (x_range == cv::Range::all() ? img.cols : x_range.end);
  for (int x = x_from; x < x_to; ++x)
    img.at<cv::Vec3b>(line_y, x) = kMagenta;
}

void DrawHorizontalSegment(cv::Mat& img, const cv::Vec3b& color, int line_y,
                           cv::Range x_range = cv::Range::all()) {
  int x_from = (x_range == cv::Range::all() ? 0 : x_range.start);
  int x_to = (x_range == cv::Range::all() ? img.cols : x_range.end);
  for (int x = x_from; x < x_to; ++x)
    img.at<cv::Vec3b>(line_y, x) = color;
}

void DrawVerticalSegment(cv::Mat& img, const cv::Vec3b& color, int line_x,
                         cv::Range y_range = cv::Range::all()) {
  int y_from = (y_range == cv::Range::all() ? 0 : y_range.start);
  int y_to = (y_range == cv::Range::all() ? img.rows : y_range.end);
  for (int y = y_from; y < y_to; ++y)
    img.at<cv::Vec3b>(y, line_x) = color;
}

void DrawRectangle(cv::Mat& img, const Rectangle& r, const cv::Vec3b& color) {
  DrawHorizontalSegment(img, color, r.first.y(),
                        cv::Range(r.first.x(), r.second.x()));
  DrawHorizontalSegment(img, color, r.second.y(),
                          cv::Range(r.first.x(), r.second.x()));
  DrawVerticalSegment(img, color, r.first.x(),
                      cv::Range(r.first.y(), r.second.y()));
  DrawVerticalSegment(img, color, r.second.x(),
                        cv::Range(r.first.y(), r.second.y()));
}

void DrawComponent(cv::Mat& img_draw, const Component& comp,
                   const cv::Vec3b& color, const cv::Vec3b& bb_color) {
  BOOST_FOREACH(const PixelPosition& pp, comp.pixel_positions()) {
    img_draw.at<cv::Vec3b>(pp.y(), pp.x()) = color;
  }
  DrawRectangle(img_draw, comp.bounding_box(), bb_color);
}


class ComponentSizeGreater {
private:
  PixelPosition::Less px_less_;
public:
  bool operator()(const Component& lhs, const Component& rhs) const {
    if (lhs.size() != rhs.size())
      return lhs.size() > rhs.size();
    int lhs_bb_area = lhs.bounding_box().area();
    int rhs_bb_area = rhs.bounding_box().area();
    return lhs_bb_area > rhs_bb_area ||
        (lhs_bb_area == rhs_bb_area &&
            px_less_(*lhs.pixel_positions().begin(),
                     *rhs.pixel_positions().begin()));

  }
};

void ShowConnectedComponents(const cv::Mat& img_binary,
                             const cv::Mat& img,
                             const std::vector<Component>& components,
                             std::size_t max_show_count,
                             bool incremental = false) {
  const cv::Vec3b kCyan(0xFF, 0xFF, 0);
  const cv::Vec3b kMagenta(0xFF, 0, 0xFF);
  std::vector<Component> comps = components;
  std::sort(comps.begin(), comps.end(), ComponentSizeGreater());

  std::size_t itr = 0;
  cv::Mat img_draw_all;
  if (!incremental)
    img_draw_all = img.clone();
  BOOST_FOREACH(const Component& comp, comps) {
    std::cout << "Component size: " << comp.size() << std::endl;
    if (incremental) {
      cv::Mat img_draw_one = img.clone();
      DrawComponent(img_draw_one, comp, kCyan, kMagenta);
      cv::imshow("flood-fill component", img_draw_one);
      cv::waitKey(-1);
    } else {
      DrawComponent(img_draw_all, comp, kCyan, kMagenta);
    }
    if (++itr == max_show_count)
      break;
  }
  if (!incremental) {
    cv::imshow("connected components", img_draw_all);
    cv::waitKey(-1);
  }
}

std::string GetCompBlackWhiteRatioName(std::size_t ind) {
  return MakeString() << "b/w ratio #" << ind;
}

std::string GetCompCentroidXName(std::size_t ind) {
  return MakeString() << "comp centroid x #" << ind;
}

std::string GetCompCentroidYName(std::size_t ind) {
  return MakeString() << "comp centroid y #" << ind;
}


std::vector<float> GetComponentRectangularities(
    const std::vector<Component> comps) {
  std::vector<float> rectangularities;
  BOOST_FOREACH(const Component& comp, comps) {
    rectangularities.push_back(comp.rectangularity());
  }
  return rectangularities;
}


}  // namespace


void ComputePlateDiscriminationStatistics(
    const boost::filesystem::path& file_path) {
  std::fstream ifs;
  ifs.open(file_path.string().c_str());

  MeasurementMap mm_cur, mm_all, mm_group_avg, mm_group_stddev,
  mm_group_relerr, mm_group_maxdiff;

  std::size_t kVerBWRatioCount;
  std::cin >> kVerBWRatioCount;

  for (std::string s; ifs >> s; ) {
    if (s[0] == '-') {
      PrintStatistics(mm_cur, "Group average");
      std::cout << std::endl;
      DoAfterGroup(&mm_cur, &mm_all, &mm_group_avg, &mm_group_stddev,
                   &mm_group_relerr, &mm_group_maxdiff);
      continue;
    }

    ImagePath img_path(s);
//    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.5);
    cv::Mat img = PlateDiscriminator::LoadAsGrayscale(img_path);

    cv::Mat img_filtered = PlateDiscriminator::ApplyFilters(img);
    cv::Mat img_filtered_trr = RemoveThinRegions(
        img_filtered, PlateDiscriminator::NonCharacterRegionsRemover(),
        PlateDiscriminator::kThinEdgeRemovalIterations);

    std::vector<Component> characters;
    PlateDiscriminator::ExtractCharacters(img_filtered_trr, &characters);

    mm_cur[kBlackWhiteRatio].Add(GetBlackWhiteRatio(img_filtered_trr));
    mm_cur[kCenterBlackWhiteRatio].Add(GetCenterBlackWhiteRatio(img_filtered_trr));

//    std::pair<float, float> normalized_centroid = GetNormalizedCentroids(
//        img_filtered_trr, 1)[0];
//    mm_cur[kNormCentroidX].Add(normalized_centroid.first);
//    mm_cur[kNormCentroidY].Add(normalized_centroid.second);

    // uniqueness square sum
     // s = sys.stdin.readlines(); sum(map(lambda x: x**2, map(float, map(lambda x: x.rstrip().split()[4], s))))

    //    const int kCentroidCount = 10;
//    std::vector<std::pair<float, float> > centroids
//    = GetNormalizedCentroids(img_filtered_trr, kCentroidCount);
//    for (int i = 0; i < kCentroidCount; ++i) {
//      mm_cur[GetNthCentroidName(i, 'x')].Add(centroids[i].first);
//      mm_cur[GetNthCentroidName(i, 'y')].Add(centroids[i].second);
//    }

//    std::vector<double> moments = GetMoments(img_filtered);
//    for (int i = 0; i < moments.size(); ++i) {
//      mm_cur[GetMomentName(i)].Add(moments[i]);
//    }


    std::vector<float> vertical_bw_ratios;
    std::cerr << "GetVerticalBWRatios" << std::endl;
    GetVerticalBlackWhiteRatios(img_filtered_trr, kVerBWRatioCount,
                                &vertical_bw_ratios);
    for (std::size_t i = 0; i < kVerBWRatioCount; ++i) {
      mm_cur[MakeString() << "ver BW #" <<
             std::setw(2) << std::setfill('0') << i].Add(vertical_bw_ratios[i]);
    }

    // bad uniqueness
//    const std::size_t kHorBWRatioCount = 5;
//    std::vector<float> horizontal_bw_ratios;
//    GetHorizontalBlackWhiteRatios(img_filtered_trr, kHorBWRatioCount,
//                                  &horizontal_bw_ratios);
//    for (std::size_t i = 0; i < kHorBWRatioCount; ++i) {
//      mm_cur[MakeString() << "hor BW #" << i].Add(horizontal_bw_ratios[i]);
//    }


    for (std::size_t i = 0; i < PlateDiscriminator::kMaxCharactersCount; ++i) {
      float r = (i < characters.size() ? characters[i].rectangularity() : 0);
      mm_cur[GetCompBlackWhiteRatioName(i)].Add(r);

      std::pair<float, float> c = (i < characters.size() ? characters[i].normalized_centroid<float>() : std::make_pair(0.f, 0.f));
      mm_cur[GetCompCentroidXName(i)].Add(c.first);
      mm_cur[GetCompCentroidYName(i)].Add(c.second);
    }

    std::cout << "image: " << img_path.string() << std::endl;
    PrintLastMeasurements(mm_cur);

//    cv::imshow("plate (filtered + removed thin regions))", img_filtered_trr);
//    cv::imshow("plate (filtered)", img_filtered);
//    cv::imshow("plate (filtered - extra small)", Resize(img_filtered, 0.25));

//    DrawHorizontalLine(img, kCharactersBeginHeightFactor);
//    DrawHorizontalLine(img, kCharactersEndHeightFactor);
//    cv::imshow("plate", img);
//    cv::waitKey(-1);

//    ShowConnectedComponents(img_filtered_trr, img, characters, false);

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

void TestPlateFilters(const boost::filesystem::path& file_path) {

  std::fstream ifs;
  ifs.open(file_path.string().c_str());


  MeasurementMap mm_cur, mm_all, mm_group_avg, mm_group_stddev,
  mm_group_relerr, mm_group_maxdiff;

  for (std::string s; ifs >> s; ) {
    if (s[0] == '-') {
      continue;
    }

    ImagePath img_path(s);
    cv::Mat img = Resize(ImageLoader::instance().LoadImage(img_path), 0.5);
    std::cout << "image: " << img_path.string() << std::endl;
    std::cout << std::endl;
    //      cv::imshow(img_path.string(), img);

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::imshow("plate-test", img_gray);

    cv::Mat eq_hist = PlateDiscriminator::Equalize(img_gray);
    cv::imshow("eq. hist.", eq_hist);

    cv::Mat img_thres = PlateDiscriminator::AdaptiveThresholdMean(
        eq_hist,
        PlateDiscriminator::kAdaptiveThresholdC);
    cv::imshow("eq. hist. + adapt. thres.", img_thres);

//    TestRemoveThinRegions4(img_thres);

    cv::Mat img_trr = RemoveThinRegions(
        img_thres,
        PlateDiscriminator::NonCharacterRegionsRemover(),
        PlateDiscriminator::kThinEdgeRemovalIterations);

    cv::imshow("eq. hist. + adapt. thres. + trr", img_trr);

    std::vector<Component> characters;
    PlateDiscriminator::ExtractCharacters(img_trr, &characters);

    ShowConnectedComponents(img_trr, img, characters, false);

    //    DrawHorizontalLine(img, kCharactersBeginHeightFactor);
    //    DrawHorizontalLine(img, kCharactersEndHeightFactor);
    //    cv::imshow("plate", img);
    //    cv::waitKey(-1);

  }
  ifs.close();
}

}  // namespace test

}  // namespace slesann
