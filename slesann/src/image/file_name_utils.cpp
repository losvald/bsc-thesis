/*
 * file_name_utils.cpp
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
 * file_utils.cpp
 *
 *  Created on: May 12, 2011
 *      Author: losvald
 */

#include "file_name_utils.h"

namespace slesann {

namespace {
  std::set<std::string> kSupportedExtensions;

  std::set<std::string> CreateSupportedExtensions() {
    std::set<std::string> s;
    s.insert("jpg");
    s.insert("jpeg");
    return s;
  }
}

const char* kCheckpointSeparator = "-";

std::string GetCheckpointName(const boost::filesystem::path& p) {
  return GetCheckpointName(fs::basename(p));
}

std::string GetCheckpointName(const std::string& file_basename) {
  std::string::size_type from = file_basename.find(kCheckpointSeparator);
  std::string::size_type to = file_basename.find(kCheckpointSeparator, from + 1);
  return file_basename.substr(from + 1, to - from - 1);
}

const std::set<std::string>& SupportedImageExtensions() {
  // XXX not thread safe
  if (kSupportedExtensions.empty()) {
    kSupportedExtensions = CreateSupportedExtensions();
  }
  return kSupportedExtensions;
}

}  // namespace slesann
