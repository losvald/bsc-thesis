/*
 * loadimage.cpp
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
 * loadimage.cpp
 *
 *  Created on: Apr 2, 2011
 *      Author: losvald
 */

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#if 0
int main(int argc, char **argv) {

  using namespace std;
  namespace fs = boost::filesystem;

  string filename;
  cin >> filename;

  fs::path p(filename);

  if (!fs::exists(p)) {
    cerr << "File " << p << " does not exists" << endl;
    return 1;
  }

  string canon_filename = p.string();

  cv::Mat img_mat = cv::imread(canon_filename, -1);


  cv::imshow("foo", img_mat);
  cv::waitKey(0);

  return 0;
}

#endif
