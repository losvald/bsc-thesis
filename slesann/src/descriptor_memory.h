/*
 * descriptor_memory.h
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
 * descriptor_memory.h
 *
 *  Created on: Apr 5, 2011
 *      Author: losvald
 */

#ifndef DESCRIPTOR_MEMORY_H_
#define DESCRIPTOR_MEMORY_H_

template<typename T>
struct DefaultDescriptorAllocator {
  void operator()(const T& vehicle) const {
  }
};

template<typename T>
struct DefaultDescriptorDeallocator {
  void operator()(const T& vehicle) const {
  }
};

#endif /* DESCRIPTOR_MEMORY_H_ */
