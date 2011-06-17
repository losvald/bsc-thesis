/*
 * plate_discriminator.h
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
 * plate_discriminator.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef PLATE_DISCRIMINATOR_H_
#define PLATE_DISCRIMINATOR_H_

#include "../discriminator.h"
#include "component_utils.h"
#include "image_base.h"

#include "binary_image_utils.h"

namespace slesann {

//class Component;

class PlateDiscriminator : public Discriminator<ImagePath> {

public:
  static const double kNormCharBeginY;
  static const double kNormCharEndY;
  static const float kMinNormCharHeight;
  static const float kMaxNormCharWidth;
  static const double kAdaptiveThresholdC;
  static const std::size_t kThinEdgeRemovalIterations;
  static const std::size_t kMaxCharactersCount;
  static const std::size_t kVerticalProjectionN;

  float ComputeSimilarity(const ImagePath& d1, const ImagePath& d2) const;
  float ComputeUniqueness(const ImagePath& d,
                          const std::vector<float>& match_candidates) const;

  static cv::Mat LoadAsGrayscale(const ImagePath& image_path);


  static cv::Mat ApplyFilters(const cv::Mat& img,
                              double adaptiveThresholdC = kAdaptiveThresholdC);

  static void ExtractCharacters(const cv::Mat& img_binary,
                                std::vector<Component>* comps);

  class NonCharacterRegionsRemover : public ThinRegionRemover {
  public:
    AdaptiveThresholdParamMap CreateMap(const cv::Mat& img_binary,
                                        std::size_t itr) const;

    int DetermineMaxBlockSize(const cv::Mat& img_binary,
                              std::size_t itr) const;
  };


  static cv::Mat AdaptiveThresholdMean(const cv::Mat& img_gray, double c);
  static cv::Mat Equalize(const cv::Mat& img_gray);


};

}  // namespace slesann

#endif /* PLATE_DISCRIMINATOR_H_ */
