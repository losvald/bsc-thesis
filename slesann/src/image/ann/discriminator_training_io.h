/*
 * discriminator_training_io.h
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
 * discriminator_training_io.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef DISCRIMINATOR_TRAINING_IO_H_
#define DISCRIMINATOR_TRAINING_IO_H_

#include <fstream>

#include "image_matching_trainer.h"

#include "../image_base.h"

namespace slesann {

namespace ann {

namespace {

template<bool T>
void LoadDataSet(const boost::filesystem::path& file_path,
                     ImageMatchingTrainer<T>* image_matching_trainer,
                     bool training) {
  std::ifstream ifs;
  ifs.open(file_path.string().c_str());
  std::string img_path1_str, img_path2_str; float output;
  while (ifs >> img_path1_str >> img_path2_str >> output) {
    ImagePath img_path1(img_path1_str), img_path2(img_path2_str);
    if (training)
      image_matching_trainer->AddForTrain(img_path1, img_path2, output);
    else
      image_matching_trainer->AddForTest(img_path1, img_path2, output);
  }
  ifs.close();
}

}  // namespace

template<bool T>
void LoadTrainDataSet(const boost::filesystem::path& file_path,
                     ImageMatchingTrainer<T>* image_matching_trainer) {
  LoadDataSet(file_path, image_matching_trainer, true);
}

template<bool T>
void LoadTestDataSet(const boost::filesystem::path& file_path,
                     ImageMatchingTrainer<T>* image_matching_trainer) {
  LoadDataSet(file_path, image_matching_trainer, false);
}

}  // namespace ann

}  // namespace slesann

#endif /* DISCRIMINATOR_TRAINING_IO_H_ */
