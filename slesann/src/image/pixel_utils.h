/*
 * pixel_utils.h
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
 * pixel_utils.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef PIXEL_UTILS_H_
#define PIXEL_UTILS_H_

#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "image_base.h"

namespace slesann {

class PixelPosition {
public:
  static const PixelPosition kNegInf;
  static const PixelPosition kPosInf;

  PixelPosition();
  PixelPosition(int x, int y);

  template<typename T>
  std::pair<T, T> Normalized(int x_min, int y_min, int x_max, int y_max) const {
    return std::make_pair(static_cast<T>(x_ - x_min) / (x_max - x_min + 1),
                          static_cast<T>(y_ - y_min) / (y_max - y_min + 1));
  }

  template<typename T>
  std::pair<T, T> Normalized(const PixelPosition& min,
                             const PixelPosition& max) const {
    return Normalized<T>(min.x_, min.y_, max.x_, max.y_);
  }

  friend PixelPosition& operator+=(PixelPosition& lhs, const PixelPosition& rhs);
  friend PixelPosition operator+(const PixelPosition& lhs,
                                 const PixelPosition& rhs);
  friend PixelPosition& operator-=(PixelPosition& lhs, const PixelPosition& rhs);
  friend PixelPosition operator-(const PixelPosition& lhs,
                                 const PixelPosition& rhs);
  friend PixelPosition& operator*=(PixelPosition& lhs, int rhs);
  friend PixelPosition operator*(const PixelPosition& lhs, int rhs);
  friend PixelPosition& operator*=(PixelPosition& lhs, float rhs);
  friend PixelPosition operator*(const PixelPosition& lhs, float rhs);
  friend PixelPosition& operator/=(PixelPosition& lhs, int rhs);
  friend PixelPosition operator/(const PixelPosition& lhs, int rhs);
  friend PixelPosition& operator/=(PixelPosition& lhs, float rhs);
  friend PixelPosition operator/(const PixelPosition& lhs, float rhs);
  friend std::ostream& operator<<(std::ostream& os, const PixelPosition& pp);
  int operator[](std::size_t n) const;
  int x() const;
  int y() const;
  int& x();
  int& y();
  PixelPosition neighbor(std::size_t n) const;

  struct Less {
    bool operator()(const PixelPosition& lhs, const PixelPosition& rhs) const;
  };
private:
  int x_;
  int y_;
};

class Rectangle : public std::pair<PixelPosition, PixelPosition> {
  typedef std::pair<PixelPosition, PixelPosition> super;
public:
  Rectangle();
  Rectangle(const PixelPosition& upper_left_corner,
            const PixelPosition& lower_right_corner);
  Rectangle(int upper_left_corner_x, int upper_left_corner_y,
            int lower_right_corner_x, int lower_right_corner_y);
  Rectangle(int upper_left_corner_x, int upper_left_corner_y,
            const PixelPosition& lower_right_corner);
  Rectangle(const PixelPosition& upper_left_corner,
            int lower_right_corner_x, int lower_right_corner_y);
  friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
  int width() const;
  int height() const;
  int area() const;
  int perimeter() const;
  PixelPosition center() const;
  bool FitsInside(const Rectangle& other) const;
  bool IsValid() const;
};

enum HullSizeComparisonMethod {
  kBoundingBox = 0
};

class Hull {
public:
  Hull();
  void Add(const PixelPosition& pp);
  bool Contains(const PixelPosition& pp) const;
  bool GreedyInside(const PixelPosition& pp) const;
  const PixelPosition& operator[](std::size_t n) const;
  friend std::ostream& operator<<(std::ostream& os, const Hull& h);
  const std::vector<PixelPosition>& pixel_positions() const;
  int convex_area() const;
//  int area() const;
  int perimeter() const;
  const Rectangle& bounding_box() const;
private:
  std::vector<PixelPosition> pixels_;
  std::map<int, std::set<int> > x_map_, y_map_;
  Rectangle bounding_box_;
};

inline int ComputeMid(int lo, int hi) {
  return (lo + hi) >> 1;
}

std::ostream& operator<<(std::ostream& os, const Hull& h);

}  // namespace slesann

#endif /* PIXEL_UTILS_H_ */
