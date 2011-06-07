/*
 * file_utils.cpp
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
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#include "base.h"
#include "file_utils.h"

#include <boost/filesystem.hpp>

namespace slesann {

void RequireExists(const fs::path& p) {
  if (!fs::exists(p))
    PrintError(EXIT_FAILURE, (!fs::is_directory(p) ? "Filename" : "Directory") +
               std::string(" \"") + p.string() + "\" does not exist");
}

void RequireEmpty(const fs::path& p) {
  if (!fs::is_empty(p))
    PrintError(EXIT_FAILURE, "\"" + p.string() + "\" is not empty");
}

void RequireDirectory(const fs::path& p) {
  if (!fs::is_directory(p))
    PrintError(EXIT_FAILURE, "\"" + p.string() + "\" is not a directory");
}

void RequireRegularFile(const fs::path& p) {
  if (!fs::is_regular_file(p))
     PrintError(EXIT_FAILURE, "\"" + p.string() + "\" is not a regular file");
}

fs::path AsInputFile(const std::string& s) {
  fs::path file_path(s);
  RequireExists(file_path);
  return file_path;
}

fs::path AsOutputFile(const std::string& s, bool overwrite) {
  fs::path file_path(s);
  if (fs::exists(file_path)) {
    if (!overwrite)
      PrintError(EXIT_FAILURE, "File \"" + file_path.string() + "\" already"
                 " exists");
    else
      RequireRegularFile(file_path);
  }
  return file_path;
}

fs::path AsDirectory(const std::string& s) {
  fs::path dir_path(s);
  RequireDirectory(s);
  return dir_path;
}

fs::path AsInputDirectory(const std::string& dirName) {
  namespace fs = boost::filesystem;
  fs::path dir(dirName);
  RequireExists(dir);
  RequireDirectory(dir);
  return dir;
}

fs::path AsOutputDirectory(const std::string& s, bool require_empty) {
  fs::path dir_path(s);
  RequireDirectory(dir_path);
  if (fs::exists(dir_path)) {
    if (require_empty)
      RequireEmpty(dir_path);
  } else {
    fs::create_directories(dir_path);
  }
  return dir_path;
}

}  // namespace slesann
