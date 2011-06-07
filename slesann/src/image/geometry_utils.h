/*
 * geometry_utils.h
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
 * geometry_utils.h
 *
 *  Created on: May 18, 2011
 *      Author: losvald
 */

#ifndef GEOMETRY_UTILS_H_
#define GEOMETRY_UTILS_H_

#include <algorithm>
#include <queue>
#include <set>
#include <utility>

#include <boost/foreach.hpp>

#include "image_base.h"
#include "pixel_utils.h"

#include <cassert>

namespace slesann {

namespace  {

void AddSubregion(const Rectangle& r, std::queue<Rectangle>* q) {
  if (r.IsValid())
    q->push(r);
}

void AddSubregions(const Rectangle& r, int mid_x, int mid_y,
                   std::queue<Rectangle>* q) {
  int w = r.width(), h = r.height();
  if (w > 1 && h > 1) {
    assert(mid_x >= r.first.x() && mid_x <= r.second.x());
    assert(mid_y >= r.first.y() && mid_y <= r.second.y());

    AddSubregion(Rectangle(r.first, PixelPosition(mid_x - 1, mid_y - 1)), q);
    AddSubregion(Rectangle(PixelPosition(mid_x + 1, mid_y + 1), r.second), q);
    AddSubregion(Rectangle(PixelPosition(mid_x + 1, r.first.y()),
                           PixelPosition(r.second.x(), mid_y - 1)), q);
    AddSubregion(Rectangle(PixelPosition(r.first.x(), mid_y + 1),
                           PixelPosition(mid_x - 1, r.second.y())), q);
  } else if (w > 1) {
    AddSubregion(Rectangle(r.first, PixelPosition(mid_x - 1, r.second.y())), q);
    AddSubregion(Rectangle(PixelPosition(mid_x + 1, r.first.y()), r.second), q);
  } else if (h > 1) {
    AddSubregion(Rectangle(r.first, PixelPosition(r.second.x(), mid_y - 1)), q);
    AddSubregion(Rectangle(PixelPosition(r.first.x(), mid_y + 1), r.second), q);
  }
}

void RelaxMax(const Hull& candidate, Hull* max) {
  if (candidate.bounding_box().area() > max->convex_area())
    *max = candidate;
}

void AddBoundaryPixels(
    const Hull& h,
    std::set<PixelPosition, PixelPosition::Less>* boundary_pixels) {
  const std::vector<PixelPosition>& v = h.pixel_positions();
  boundary_pixels->insert(v.begin(), v.end());
}

}  // namespace



template<class TBoundaryDetectionFunctor>
Hull HullBfs(const PixelPosition& edge_pixel,
             const TBoundaryDetectionFunctor& f) {
//  std::cerr << "Hullbfs\n";
  std::queue<PixelPosition> q;
  std::set<PixelPosition, PixelPosition::Less> visited;
  q.push(edge_pixel);
  visited.insert(edge_pixel);
  for (; !q.empty(); q.pop()) {
    const PixelPosition& u = q.front();
    for (std::size_t i = 0; i < 8; i += 2) {
      PixelPosition n = u.neighbor(i);
      if (!visited.count(n) && f(n.x(), n.y())) {
        visited.insert(n);
        q.push(n);
      }
    }
  }

  Hull h;
  BOOST_FOREACH(const PixelPosition& pp, visited) {
    h.Add(pp);
  }
  return h;
}

template<class TBoundaryDetectionFunctor>
Hull FindGreatestHull(const cv::Mat& img, const TBoundaryDetectionFunctor& f) {
  Hull h_max;

  std::queue<Rectangle> q;
  std::set<PixelPosition, PixelPosition::Less> boundary_pixels;
  q.push(Rectangle(PixelPosition(0, 0),
                   PixelPosition(img.cols - 1, img.rows - 1)));

  for (; !q.empty(); q.pop()) {
    const Rectangle& r = q.front();
    if (!h_max.bounding_box().FitsInside(r))
      continue;

    // check for boundaries on vertical line
    int mid_x = ComputeMid(r.first.x(), r.second.x());
    for (int y = r.first.y(); y < r.second.y(); ++y)
      if (f(mid_x, y) && !boundary_pixels.count(PixelPosition(mid_x, y))) {
        Hull h = HullBfs(PixelPosition(mid_x, y), f);
        AddBoundaryPixels(h, &boundary_pixels);
        RelaxMax(h, &h_max);
      }
    // check for boundaries on horizontal line
    int mid_y = ComputeMid(r.first.y(), r.second.y());
    for (int x = r.first.x(); x < r.second.x(); ++x)
      if (f(x, mid_y) && !boundary_pixels.count(PixelPosition(x, mid_y))) {
        Hull h = HullBfs(PixelPosition(x, mid_y), f);
        AddBoundaryPixels(h, &boundary_pixels);
        RelaxMax(h, &h_max);
      }

    AddSubregions(r, mid_x, mid_y, &q);
  }

  return h_max;
}



}  // namespace slesann

#endif /* GEOMETRY_UTILS_H_ */
