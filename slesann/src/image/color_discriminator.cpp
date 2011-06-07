/*
 * color_discriminator.cpp
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
 * color_discriminator.cpp
 *
 *  Created on: May 27, 2011
 *      Author: losvald
 */

#include <iomanip>

#include <boost/foreach.hpp>

#include "../util/sparse_multiset.h"
#include "../util/string_utils.h"
#include "color_discriminator.h"
#include "image_loader.h"
#include "transform_utils.h"

namespace slesann {

namespace {

typedef SparseMultiset<uchar, 181, true> HueMultiset;
//typedef SparseMultiset<uchar, 0xFF, false> SaturationMultiset;
typedef SparseMultiset<uchar, 0x100, false> IntensityMultiset;

class HeuristicYFilter {
  ;
  int y_windshield_begin_;
  int y_windshield_end_;
  int y_bumper_begin_;

  void Init(const Rectangle& bb) {
    y_windshield_begin_ = static_cast<int>(bb.first.y() + 0.05 * bb.height());
    y_windshield_end_ = static_cast<int>(bb.first.y() + 0.3 * bb.height());
    y_bumper_begin_ = static_cast<int>(bb.first.y() + 0.58 * bb.height());
  }

public:
  HeuristicYFilter(const Hull& hull) {
    Init(hull.bounding_box());
  }
  bool IsBumper(int y) const {
    return y >= y_bumper_begin_;
  }
  bool IsWindshield(int y) const {
    return y >= y_windshield_begin_ && y <= y_windshield_end_;
  }

};

void ComputeDominators0(const cv::Mat& img_hsv,
                        const std::vector<PixelPosition>& body_pixels,
                        HueMultiset* dominant_hues,
                        IntensityMultiset* dominant_intensities,
                        uchar* hue_lsf, uchar* saturation_avg,
                        uchar* intensity_avg) {
  HueMultiset all_hues;
  long long hue_sum = 0, saturation_sum = 0, intensity_sum = 0;

  BOOST_FOREACH(const PixelPosition& pp, body_pixels) {
    const cv::Vec3b px = img_hsv.at<cv::Vec3b>(pp.y(), pp.x());
    const uchar hue = px[0], saturation = px[1], intensity = px[2];
    hue_sum += hue; saturation_sum += saturation; intensity_sum += intensity;
    all_hues.Add(hue);
    if (ColorDiscriminator::IsHueDominant(saturation, intensity)) {
      dominant_hues->Add(hue);
      //          static int itr = 0;
      //          if (++itr == 10) {
      //            itr = 0;
      //            std::cout << "hue = " << (int)hue * 2 << std::endl;
      //          }
    }
    else {
      dominant_intensities->Add(intensity);
      //          static int itr = 0;
      //          if (++itr == 10) {
      //            itr = 0;
      //            std::cout << "int = " << (int)intensity << std::endl;
      //          }
    }
  }
  std::size_t total_pixels = dominant_hues->size() + dominant_intensities->size();
  *saturation_avg = saturation_sum / total_pixels;
  *intensity_avg = intensity_sum / total_pixels;
  *hue_lsf = all_hues.FindLeastSquaresFit<int>();
}

}  // namespace

const float ColorDiscriminator::kResizeFactor = 0.2;

float ColorDiscriminator::ComputeSimilarity(const ImagePath& d1,
                                            const ImagePath& d2) const {
  // TODO

  return 0;
}

float ColorDiscriminator::ComputeUniqueness(
    const ImagePath& d,const std::vector<float>& match_candidates) const {
  return *std::max_element(match_candidates.begin(),
                           match_candidates.end()) < 0.5f ? 0.f : 1.f;
}

ColorDiscriminator::Dominator::Dominator() {}

ColorDiscriminator::Dominator::Dominator(float dominance)
: dominance_(dominance) {}

float ColorDiscriminator::Dominator::dominance() const {
  return dominance_;
}

std::string ColorDiscriminator::Dominator::dominance_str() const {
  return MakeString() << std::fixed << std::setw(3) << std::setprecision(0) <<
      100 * dominance() << " %";
}

ColorDiscriminator::HueDominator::HueDominator() {}

ColorDiscriminator::HueDominator::HueDominator(uchar hue, float dominance)
: super(dominance), hue_(hue) {}

uchar ColorDiscriminator::HueDominator::hue() const {
  return hue_;
}

std::string ColorDiscriminator::HueDominator::hue_str() const {
  return HueToString(hue());
}

ColorDiscriminator::IntensityDominator::IntensityDominator() {}

ColorDiscriminator::IntensityDominator::IntensityDominator(uchar intensity,
                                                           float dominance)
: super(dominance), intensity_(intensity) {}

uchar ColorDiscriminator::IntensityDominator::intensity() const {
  return intensity_;
}

std::string ColorDiscriminator::IntensityDominator::intensity_str() const {
  return IntensityToString(intensity());
}


cv::Mat ColorDiscriminator::LoadImage(const ImagePath& img_path) {
  cv::Mat img_orig_size = ImageLoader::instance().LoadImage(img_path);
  return Resize(img_orig_size, kResizeFactor);
}

uchar ColorDiscriminator::GetHueDominanceThreshold(uchar intensity) {
  // threshold = 1 - 0.8 * V
  return 255 - static_cast<uchar>(0.8f * intensity);
}

bool ColorDiscriminator::IsHueDominant(uchar saturation, uchar intensity) {
  return //15
      saturation > GetHueDominanceThreshold(intensity);
}

void ColorDiscriminator::ComputeDominators(
    const cv::Mat& img_hsv, const std::vector<PixelPosition>& body_pixels,
    HueDominator* hue_dominator, IntensityDominator* intensity_dominator,
    uchar* hue_lsf, uchar* saturation_avg, uchar* intensity_avg) {
  HueMultiset dom_hues;
  IntensityMultiset dom_intensities;

  ComputeDominators0(img_hsv, body_pixels, &dom_hues, &dom_intensities,
                     hue_lsf, saturation_avg, intensity_avg);
  float hue_dominance = static_cast<float>(dom_hues.size()) /
                                           (dom_hues.size() + dom_intensities.size());
  *hue_dominator = HueDominator(dom_hues.FindLeastSquaresFit<int>(),
                                hue_dominance);
  *intensity_dominator = IntensityDominator(dom_intensities.FindMedian(),
                                            1 - hue_dominance);
}

cv::Vec3b ColorDiscriminator::GetAverageBgrColor(
      const cv::Mat& img, const std::vector<PixelPosition>& body_pixels) {
  cv::Vec3i sum;
  BOOST_FOREACH(const PixelPosition& pp, body_pixels) {
    const cv::Vec3b& px = img.at<cv::Vec3b>(pp.y(), pp.x());
    sum += px;
  }
  return cv::Vec3b(sum[0] / body_pixels.size(),
                   sum[1] / body_pixels.size(),
                   sum[2] / body_pixels.size());
}

void ColorDiscriminator::GetBodyPixels(const cv::Mat& img_hsv, const Hull& hull,
                                       std::vector<PixelPosition>* body_pps) {
  HeuristicYFilter y_filter(hull);
  std::size_t max_pixels = 0;
  for (int r = 0; r < img_hsv.rows; ++r) {
    if (!(y_filter.IsBumper(r) || y_filter.IsWindshield(r)))
      max_pixels += img_hsv.cols;
  }

  body_pps->clear();
  body_pps->reserve(max_pixels);
  for (int r = 0; r < img_hsv.rows; ++r) {
    if (y_filter.IsBumper(r) || y_filter.IsWindshield(r))
        continue;
    for (int c = 0; c < img_hsv.cols; ++c) {
      const cv::Vec3b px = img_hsv.at<cv::Vec3b>(r, c);
      PixelPosition pp = PixelPosition(c, r);
      if (!kEmptyColor.IsEmpty<kHsv>(px) && hull.GreedyInside(pp))
        body_pps->push_back(pp);
    }
  }
}

} // namespace slesann
