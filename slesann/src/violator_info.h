/*
 * violator_info.h
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
 * violator_info.h
 *
 *  Created on: Apr 5, 2011
 *      Author: losvald
 */

#ifndef VIOLATOR_INFO_H_
#define VIOLATOR_INFO_H_

namespace slesann {

template<typename T>
class ViolatorInfo {

public:
  typedef T DescriptorType;

  ViolatorInfo(const T& desc_at_start,
               const T& desc_at_end);

  const T& descriptor_at_start() const;
  const T& descriptor_at_end() const;

private:

  T desc_at_start_;
  T desc_at_end_;
};


template<typename T>
ViolatorInfo<T>::ViolatorInfo(const T& desc_at_start, const T& desc_at_end)
: desc_at_start_(desc_at_start), desc_at_end_(desc_at_end) {}

template<typename T> const T& ViolatorInfo<T>::descriptor_at_start() const {
  return desc_at_start_;
}

template<typename T> const T& ViolatorInfo<T>::descriptor_at_end() const {
  return desc_at_end_;
}


}  // namespace slesann



#endif /* VIOLATOR_INFO_H_ */
