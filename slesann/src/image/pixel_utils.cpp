/*
 * pixel_utils.cpp
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
 * pixel_utils.cpp
 *
 *  Created on: May 18, 2011
 *      Author: losvald
 */

#include "pixel_utils.h"

#include <boost/foreach.hpp>

namespace slesann {

namespace {

const int kInf = 0x3f3f3f3f;

}  // namespace

const PixelPosition PixelPosition::kPosInf(kInf, kInf);
const PixelPosition PixelPosition::kNegInf(-kInf, -kInf);

PixelPosition::PixelPosition() : x_(0), y_(0) {}

PixelPosition::PixelPosition(int x, int y) : x_(x), y_(y) {}

PixelPosition& operator+=(PixelPosition& lhs, const PixelPosition& rhs) {
  lhs.x_ += rhs.x_;
  lhs.y_ += rhs.y_;
  return lhs;
}

PixelPosition operator+(const PixelPosition& lhs, const PixelPosition& rhs) {
  PixelPosition ret = lhs;
  return ret += rhs;
}

PixelPosition& operator-=(PixelPosition& lhs, const PixelPosition& rhs) {
  lhs.x_ -= rhs.x_;
  lhs.y_ -= rhs.y_;
  return lhs;
}

PixelPosition operator-(const PixelPosition& lhs, const PixelPosition& rhs) {
  PixelPosition ret = lhs;
  return ret -= rhs;
}

PixelPosition& operator*=(PixelPosition& lhs, int rhs) {
  lhs.x_ *= rhs;
  lhs.y_ *= rhs;
  return lhs;
}

PixelPosition operator*(const PixelPosition& lhs, int rhs) {
  PixelPosition ret = lhs;
  return ret *= rhs;
}

PixelPosition& operator*=(PixelPosition& lhs, float rhs) {
  lhs.x_ = lhs.x_ * rhs + 0.5f;
  lhs.y_ = lhs.y_ * rhs + 0.5f;
  return lhs;
}

PixelPosition operator*(const PixelPosition& lhs, float rhs) {
  PixelPosition ret = lhs;
  return ret *= rhs;
}

PixelPosition& operator/=(PixelPosition& lhs, int rhs) {
  lhs.x_ /= rhs;
  lhs.y_ /= rhs;
  return lhs;
}

PixelPosition operator/(const PixelPosition& lhs, int rhs) {
  PixelPosition ret = lhs;
  return ret /= rhs;
}

PixelPosition& operator/=(PixelPosition& lhs, float rhs) {
  lhs.x_ = lhs.x_ / rhs + 0.5f;
  lhs.y_ = lhs.y_ / rhs + 0.5f;
  return lhs;
}

PixelPosition operator/(const PixelPosition& lhs, float rhs) {
  PixelPosition ret = lhs;
  return ret /= rhs;
}

int PixelPosition::operator[](std::size_t n) const {
  return (n & 1) ? y_ : x_;
}

std::ostream& operator<<(std::ostream& os, const PixelPosition& pp) {
  os << '(' << pp.x_ << ", " << pp.y_ << ')';
  return os;
}

int PixelPosition::x() const {
  return x_;
}

int PixelPosition::y() const {
  return y_;
}

int& PixelPosition::x() {
  return x_;
}

int& PixelPosition::y() {
  return y_;
}

PixelPosition PixelPosition::neighbor(std::size_t n) const {
  static const PixelPosition delta[] = {PixelPosition( 1,  0),
                                        PixelPosition( 1,  1),
                                        PixelPosition( 0,  1),
                                        PixelPosition(-1,  1),
                                        PixelPosition(-1,  0),
                                        PixelPosition(-1, -1),
                                        PixelPosition( 0, -1),
                                        PixelPosition( 1, -1)};
  return *this + delta[n];
}

bool PixelPosition::Less::operator()(const PixelPosition& lhs,
                                     const PixelPosition& rhs) const {
  return lhs.x_ < rhs.x_ || (lhs.x_ == rhs.x_ && lhs.y_ < rhs.y_);
}


Rectangle::Rectangle() {}

Rectangle::Rectangle(const PixelPosition& upper_left_corner,
                     const PixelPosition& lower_right_corner)
: super(upper_left_corner, lower_right_corner) {}


Rectangle::Rectangle(int upper_left_corner_x, int upper_left_corner_y,
          int lower_right_corner_x, int lower_right_corner_y)
: super(PixelPosition(upper_left_corner_x, upper_left_corner_y),
        PixelPosition(lower_right_corner_x, lower_right_corner_y)) {}

Rectangle::Rectangle(int upper_left_corner_x, int upper_left_corner_y,
          const PixelPosition& lower_right_corner)
: super(PixelPosition(upper_left_corner_x, upper_left_corner_y),
        lower_right_corner) {}

Rectangle::Rectangle(const PixelPosition& upper_left_corner,
                     int lower_right_corner_x, int lower_right_corner_y)
: super(upper_left_corner,
        PixelPosition(lower_right_corner_x, lower_right_corner_y)) {}

std::ostream& operator<<(std::ostream& os, const Rectangle& r) {
  os << '[' << r.first << ", " << r.second << ']';
  return os;
}

int Rectangle::width() const {
  return second.x() - first.x() + 1;
}

int Rectangle::height() const{
  return second.y() - first.y() + 1;
}

int Rectangle::area() const {
  return width() * height();
}

int Rectangle::perimeter() const {
  return 2 * (width() + height());
}

bool Rectangle::FitsInside(const Rectangle& other) const {
  return height() <= other.height() && width() <= other.width();
}

bool Rectangle::IsValid() const {
  return first.x() <= second.x() && first.y() <= second.y();
}

PixelPosition Rectangle::center() const {
    return PixelPosition(ComputeMid(first.x(), second.x()),
                         ComputeMid(first.y(), second.y()));
}

Hull::Hull() : bounding_box_(PixelPosition::kPosInf, PixelPosition::kNegInf) {}

void Hull::Add(const PixelPosition& pp) {
  if (Contains(pp))
    return ;

  pixels_.push_back(pp);

  x_map_[pp.x()].insert(pp.y());
  y_map_[pp.y()].insert(pp.x());

  bounding_box_.first.x() = std::min(bounding_box_.first.x(), pp.x());
  bounding_box_.first.y() = std::min(bounding_box_.first.y(), pp.y());
  bounding_box_.second.x() = std::max(bounding_box_.second.x(), pp.x());
  bounding_box_.second.y() = std::max(bounding_box_.second.y(), pp.y());
}

bool Hull::Contains(const PixelPosition& pp) const {
  return y_map_.count(pp.y()) && y_map_.at(pp.y()).count(pp.x());
}

bool Hull::GreedyInside(const PixelPosition& pp) const {
  return y_map_.count(pp.y()) &&
      *y_map_.at(pp.y()).begin() <= pp.x() &&
      pp.x() <= *--y_map_.at(pp.y()).end();
}

const PixelPosition& Hull::operator[](std::size_t n) const {
  return pixels_[n];
}

std::ostream& operator<<(std::ostream& os, const Hull& h) {
  os << '[';
  for (std::size_t i = 0, to = h.pixels_.size(); i < to; ++i) {
    if (i) os << ", ";
    os << h[i];
  }
  os << ']';
  return os;
}

const std::vector<PixelPosition>& Hull::pixel_positions() const {
  return pixels_;
}

const Rectangle& Hull::bounding_box() const {
  return bounding_box_;
}

//int Hull::area() const {
//  int ret = 0;
//  typedef std::map<int, std::set<int> > Map;
//  for (Map::const_iterator it = y_map_.begin(), end = y_map_.end();
//      it != end; ++it) {
//    bool odd = false;
//    int last_x = 0;
//    BOOST_FOREACH(int x, it->second) {
//      if (!(odd ^= true))
//        ret += x - last_x;
//      last_x = x;
//    }
//  }
//  return ret;
//}

int Hull::convex_area() const {
  int ret = 0;
  typedef std::map<int, std::set<int> > Map;
  for (Map::const_iterator it = y_map_.begin(), end = y_map_.end();
      it != end; ++it) {
    if (it->second.size() > 1)
      ret += *--(it->second.end()) - *it->second.begin();
    else
      ++ret;
  }
  return ret;
}

int Hull::perimeter() const {
  return pixels_.size();
}

}  // namespace slesann
