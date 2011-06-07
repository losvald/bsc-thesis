/*
 * match_display_test.cpp
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
 * match_display_test.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#include <algorithm>
#include <iostream>
#include <fstream>

#include <opencv/highgui.h>

#include "../../src/image/match_display.h"

namespace slesann {

namespace test {

namespace  {


}  // namespace

void ShowMatchImages(const boost::filesystem::path& file_path) {
  std::fstream ifs;
  ifs.open(file_path.string().c_str());

  ImagePath last_img_path;

  for (std::string s; ifs >> s; ) {
    if (s[0] == '-') {
      continue;
    }

    ImagePath img_path = s;

    if (!last_img_path.empty()) {
      std::cout << last_img_path.string() << " " << last_img_path.string() <<
          std::endl;
      ShowMatchingImages(last_img_path, img_path, 0.5);
      cv::waitKey(-1);
    }

    last_img_path = img_path;
  }
}

}  // namespace test

}  // namespace slesann
