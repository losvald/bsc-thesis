/*
 * component_utils.h
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
 * component_utils.h
 *
 *  Created on: May 31, 2011
 *      Author: losvald
 */

#ifndef COMPONENT_UTILS_H_
#define COMPONENT_UTILS_H_

#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "image_base.h"
#include "pixel_utils.h"

namespace slesann {

class PixelPosition;

class Component {
public:
  Component();
  void Add(const PixelPosition& pp);
  bool Contains(const PixelPosition& pp) const;
  void Clear();
  const std::set<PixelPosition, PixelPosition::Less>& pixel_positions() const;
  int area() const;
  const Rectangle& bounding_box() const;
  std::size_t size() const;

  template<typename T>
  std::pair<T, T> normalized_centroid() const {
    if (pixels_.empty())
      return std::make_pair(.5f, .5f);
    PixelPosition sum;
    for (std::set<PixelPosition, PixelPosition::Less>::const_iterator it =
        pixels_.begin(), end = pixels_.end(); it != end; ++it) {
      sum += *it;
    }
    sum /= static_cast<int>(pixels_.size());
    return sum.Normalized<T>(bounding_box_.first, bounding_box_.second);
  }

  float rectangularity() const;

  bool IsEmpty() const;
private:

  std::set<PixelPosition, PixelPosition::Less> pixels_;
  Rectangle bounding_box_;
};

class ConnectedComponentFinder {
public:
  ConnectedComponentFinder(const cv::Mat& img_binary);

  template<typename ComponentPredicate>
  void FloodFill(const PixelPosition& seed,
                 const ComponentPredicate& comp_pred,
                 Component* c);

  bool IsValid(const PixelPosition& pp);

  cv::Mat img_binary_;
  std::vector< std::vector<int> > labels_;
  int cur_label_;
};

template<typename ComponentPredicate>
void ConnectedComponentFinder::FloodFill(const PixelPosition& seed,
                                         const ComponentPredicate& comp_pred,
                                         Component* c) {
    c->Clear();
    if (!(labels_[seed.y()][seed.x()] == -1 &&
        comp_pred(seed.x(), seed.y())))
      return ;
    std::queue<PixelPosition> q;
    c->Add(seed);
    for (q.push(seed); !q.empty(); q.pop()) {
      const PixelPosition& pp = q.front();
      for (int i = 0; i < 8; i += 2) {
        PixelPosition neigh = pp.neighbor(i);
        if (IsValid(neigh) && labels_[neigh.y()][neigh.x()] == -1 &&
            comp_pred(neigh.x(), neigh.y())) {
          labels_[neigh.y()][neigh.x()] = cur_label_;
          c->Add(neigh);
          q.push(neigh);
        }
      }
    }
    ++cur_label_;
}

template<class ComponentPredicate, class ComponentFilter>
void FindComponents(const cv::Mat& img_binary,
                    const ComponentPredicate& comp_pred,
                    const ComponentFilter& comp_filter,
                    std::vector<Component>* components) {
  components->clear();
  Component cur_comp;
  ConnectedComponentFinder finder(img_binary);
  for (int y = 0; y < img_binary.rows; ++y) {
    for (PixelPosition pp(0, y); pp.x() < img_binary.cols; ++pp.x()) {
      finder.FloodFill(pp, comp_pred, &cur_comp);
      if (!cur_comp.IsEmpty() && comp_filter(cur_comp))
        components->push_back(cur_comp);
    }
  }
}

}  // namespace slesann

#endif /* COMPONENT_UTILS_H_ */
