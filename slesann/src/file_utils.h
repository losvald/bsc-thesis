/*
 * file_utils.h
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
 * file_utils.h
 *
 *  Created on: May 15, 2011
 *      Author: losvald
 */

#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

namespace slesann {

namespace fs = boost::filesystem;

typedef std::string Filename;
typedef std::vector<Filename> FilenameList;

void RequireExists(const fs::path& p);
void RequireEmpty(const fs::path& p);
void RequireDirectory(const fs::path& p);
void RequireRegularFile(const fs::path& p);

fs::path AsInputFile(const std::string& s);
fs::path AsOutputFile(const std::string& s, bool overwrite = true);

fs::path AsDirectory(const std::string& s);
fs::path AsInputDirectory(const std::string& s);
fs::path AsOutputDirectory(const std::string& s, bool require_empty = false);

}  // namespace slesann

#endif /* FILE_UTILS_H_ */
