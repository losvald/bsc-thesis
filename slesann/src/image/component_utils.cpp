/*
 * component_utils.cpp
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
 * component_utils.cpp
 *
 *  Created on: May 31, 2011
 *      Author: losvald
 */

#include <queue>

#include "component_utils.h"
#include "pixel_utils.h"

namespace slesann {

namespace {



}  // namespace

Component::Component()
: bounding_box_(PixelPosition::kPosInf, PixelPosition::kNegInf){}

void Component::Add(const PixelPosition& pp) {
  pixels_.insert(pp);

  bounding_box_.first.x() = std::min(bounding_box_.first.x(), pp.x());
  bounding_box_.first.y() = std::min(bounding_box_.first.y(), pp.y());
  bounding_box_.second.x() = std::max(bounding_box_.second.x(), pp.x());
  bounding_box_.second.y() = std::max(bounding_box_.second.y(), pp.y());
}

bool Component::Contains(const PixelPosition& pp) const {
  return pixels_.count(pp);
}

void Component::Clear() {
  pixels_.clear();
  bounding_box_ = Rectangle(PixelPosition::kPosInf, PixelPosition::kNegInf);
}


const std::set<PixelPosition, PixelPosition::Less>&
Component::pixel_positions() const {
  return pixels_;
}

int Component::area() const {
  return pixels_.size();
}

const Rectangle& Component::bounding_box() const {
  return bounding_box_;
}

std::size_t Component::size() const {
  return pixels_.size();
}

float Component::rectangularity() const {
  return static_cast<float>(size()) / bounding_box_.area();
}

bool Component::IsEmpty() const {
  return pixels_.empty();
}


bool ConnectedComponentFinder::IsValid(const PixelPosition& pp) {
  return pp.x() >= 0 && pp.x() < img_binary_.cols &&
      pp.y() >= 0 && pp.y() < img_binary_.rows;
}

ConnectedComponentFinder::ConnectedComponentFinder(const cv::Mat& img_binary)
  : img_binary_(img_binary),
    labels_(img_binary.rows, std::vector<int>(img_binary.cols, -1)),
    cur_label_(0) {}

}  // namespace slesann
