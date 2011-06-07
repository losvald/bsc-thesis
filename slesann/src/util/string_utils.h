/*
 * string_utils.h
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
 * string_utils.h
 *
 *  Created on: May 13, 2011
 *      Author: losvald
 */

#ifndef STRING_UTILS_H_
#define STRING_UTILS_H_

#include <string>
#include <sstream>

class MakeString {
public:
   template<typename T>
   MakeString& operator<<(T const & datum) {
      buffer_ << datum;
      return *this;
   }
   operator std::string() const;
private:
   std::ostringstream buffer_;
};

std::string ToLowerCase(const std::string& s);

bool EqualsIgnoreCase(const std::string& a, const std::string& b);


#endif /* STRING_UTILS_H_ */
