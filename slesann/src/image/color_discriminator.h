/*
 * color_discriminator.h
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
 * color_discriminator.h
 *
 *  Created on: May 27, 2011
 *      Author: losvald
 */

#ifndef COLOR_DISCRIMINATOR_H_
#define COLOR_DISCRIMINATOR_H_

#include <string>
#include <utility>

#include "../discriminator.h"
#include "image_base.h"
#include "pixel_utils.h"

namespace slesann {

class ColorDiscriminator : public Discriminator<ImagePath> {

public:

  static const float kResizeFactor;

  class Dominator {
  public:
    Dominator();
    Dominator(float dominance);
    float dominance() const;
    std::string dominance_str() const;
  private:
    float dominance_;
  };

  class HueDominator : public Dominator {
    typedef Dominator super;
  public:
    HueDominator();
    HueDominator(uchar hue, float dominance);
//    friend std::ostream& operator<<(std::ostream& os, const HueDominator& hd);
    uchar hue() const;
    std::string hue_str() const;
  private:
    uchar hue_;
  };

  class IntensityDominator : public Dominator {
    typedef Dominator super;
  public:
    IntensityDominator();
    IntensityDominator(uchar saturation, float dominance);
//    friend std::ostream& operator<<(std::ostream& os,
//                                    const IntensityDominator& d);
    uchar intensity() const;
    std::string intensity_str() const;
  private:
    uchar intensity_;
  };

  float ComputeSimilarity(const ImagePath& d1, const ImagePath& d2) const;
  float ComputeUniqueness(const ImagePath& d,
                          const std::vector<float>& match_candidates) const;

  static cv::Mat LoadImage(const ImagePath& img_path);

  static uchar GetHueDominanceThreshold(uchar intensity);

  static bool IsHueDominant(uchar saturation, uchar intensity);

  static void ComputeDominators(const cv::Mat& img_hsv,
                                const std::vector<PixelPosition>& body_pixels,
                                HueDominator* hue_dominator,
                                IntensityDominator* intensity_dominator,
                                uchar* hue_lsf, uchar* saturation_avg,
                                uchar* intensity_avg);

  static cv::Vec3b GetAverageBgrColor(
      const cv::Mat& img, const std::vector<PixelPosition>& body_pps);

  static void GetBodyPixels(const cv::Mat& img, const Hull& hull,
                            std::vector<PixelPosition>* body_pps);

};

} // namespace slesann

#endif /* COLOR_DISCRIMINATOR_H_ */
