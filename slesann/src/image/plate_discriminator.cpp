/*
 * plate_discriminator.cpp
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
 * plate_discriminator.cpp
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#include <algorithm>
#include <map>
#include <vector>

#include <boost/foreach.hpp>
#include <opencv/cv.h>

#include "component_utils.h"
#include "image_loader.h"
#include "plate_discriminator.h"
#include "transform_utils.h"

namespace slesann {

namespace {

class CharacterComponentPredicate {
  cv::Mat img_binary_;
  int characters_begin_y_, characters_end_y_;
public:
  CharacterComponentPredicate(const cv::Mat& img_binary)
  : img_binary_(img_binary),
    characters_begin_y_(img_binary.rows * PlateDiscriminator::kNormCharBeginY),
    characters_end_y_(img_binary.rows * PlateDiscriminator::kNormCharEndY) {
  }

  bool operator()(int x, int y) const {
    return y >= characters_begin_y_ && y <= characters_end_y_ &&
        !img_binary_.at<uchar>(y, x);
  }
};

class CharacterComponentFilter {
  cv::Mat img_;
public:
  CharacterComponentFilter(const cv::Mat& img) : img_(img) {}

  bool operator()(const Component& comp) const {
    const Rectangle& bb = comp.bounding_box();
    return bb.height() >= img_.rows * PlateDiscriminator::kMinNormCharHeight &&
        bb.width() < img_.cols * PlateDiscriminator::kMaxNormCharWidth;
  }
};

struct ComponentLeftCornerXLess {
  bool operator()(const Component& lhs, const Component& rhs) const {
    return lhs.bounding_box().first.x() < rhs.bounding_box().first.x();
  }
};

int GetAdaptiveThresholdBlockSize(const cv::Mat& img) {
  return img.rows + !(img.rows & 1);
}

}  // namespace

const double PlateDiscriminator::kNormCharBeginY = 0.05;
const double PlateDiscriminator::kNormCharEndY = 0.97;

const float PlateDiscriminator::kMinNormCharHeight = 0.5;
const float PlateDiscriminator::kMaxNormCharWidth = 0.5;

const double PlateDiscriminator::kAdaptiveThresholdC = 47;

const std::size_t PlateDiscriminator::kThinEdgeRemovalIterations = 2;

const std::size_t PlateDiscriminator::kMaxCharactersCount = 8;
const std::size_t PlateDiscriminator::kVerticalProjectionN = 8;

float PlateDiscriminator::ComputeSimilarity(const ImagePath& d1,
                                      const ImagePath& d2) const {
  // TODO implement
  return 0;
}

float PlateDiscriminator::ComputeUniqueness(
    const ImagePath& d,const std::vector<float>& match_candidates) const {
  return *std::max_element(match_candidates.begin(),
                          match_candidates.end()) < 0.5f ? 0.f : 1.f;
}

cv::Mat PlateDiscriminator::LoadAsGrayscale(const ImagePath& image_path) {
  cv::Mat img = ImageLoader::instance().LoadImage(image_path);
  // TODO determine optimal size
  float resize_factor = 0.5;
  cv::Mat img_gray;
  cv::cvtColor(Resize(img, resize_factor), img_gray, CV_BGR2GRAY);
  return img_gray;
}

cv::Mat PlateDiscriminator::ApplyFilters(const cv::Mat& img_gray,
                                         double adaptiveThresholdC) {
//  cv::Mat img_gray;
//  cv::cvtColor(img, img_gray, CV_BGR2GRAY);
  return AdaptiveThresholdMean(Equalize(img_gray), adaptiveThresholdC);
}

void PlateDiscriminator::ExtractCharacters(const cv::Mat& img_binary,
                                           std::vector<Component>* comps) {
  FindComponents(img_binary, CharacterComponentPredicate(img_binary),
                 CharacterComponentFilter(img_binary), comps);
  std::sort(comps->begin(), comps->end(), ComponentLeftCornerXLess());
//  if (comps->size() > kMaxCharactersCount)
//    comps->erase(comps->begin() + kMaxCharactersCount, comps->end());
}

PlateDiscriminator::NonCharacterRegionsRemover::AdaptiveThresholdParamMap
PlateDiscriminator::NonCharacterRegionsRemover::CreateMap(const cv::Mat& img_binary,
                                                          std::size_t itr) const {
  AdaptiveThresholdParamMap thr_map;
  // TODO check image size
  thr_map[3] = 100;
  thr_map[5] = 116;
  thr_map[7] = 120;
  thr_map[9] = 128;
  thr_map[11] = 130;
  thr_map[13] = 134;
  thr_map[15] = 136;
  thr_map[17] = 148;
  BOOST_FOREACH(const AdaptiveThresholdParams& p, thr_map) {
    thr_map[p.first] += 0;
  }
  return thr_map;
}

int PlateDiscriminator::NonCharacterRegionsRemover::DetermineMaxBlockSize(
    const cv::Mat& img_binary, std::size_t itr) const {
  return std::max(1, img_binary.rows / 10 - 2*static_cast<int>(itr)) * 2 + 1;
}


cv::Mat PlateDiscriminator::Equalize(const cv::Mat& img_gray) {
  cv::Mat eq_hist;
  cv::equalizeHist(img_gray, eq_hist);
  return eq_hist;
}

cv::Mat PlateDiscriminator::AdaptiveThresholdMean(const cv::Mat& img_gray, double c) {
  cv::Mat img_thres;
  cv::adaptiveThreshold(img_gray, img_thres, 0xFF,
                        CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,
                        GetAdaptiveThresholdBlockSize(img_gray), c);
  return img_thres;
}


}  // namespace slesann
