/*
 * image_matching_trainer.h
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
 * image_matching_trainer.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef IMAGE_MATCHING_TRAINER_H_
#define IMAGE_MATCHING_TRAINER_H_

#include "../../discriminator_matching_trainer.h"
#include "../image_base.h"

namespace slesann {

namespace ann {

template<bool cacheTransformations = true>
class ImageMatchingTrainer
: public DiscriminatorMatchingTrainer<ImagePath, cacheTransformations> {
  typedef DiscriminatorMatchingTrainer<ImagePath, cacheTransformations> super;
public:
  ImageMatchingTrainer(
      typename super::InputDataTransformerSharedPtr input_data_transformer,
      const typename super::Params& params)
  : super(input_data_transformer, params) {}

  bool TrainAndTest(const boost::filesystem::path& train_data_out_file_path,
                    const boost::filesystem::path& test_data_out_file_path) {
    using std::cout;
    using std::endl;

    cout << "Running image matching training..." << endl;

    cout << "Training ANN..." << endl;
    float train_mse;
    std::size_t train_bit_fail;
    bool train_ok = Train(train_data_out_file_path,
                          super::parameters().random_weight_limit(),
                          super::parameters().max_epochs(),
                          super::parameters().epochs_between_reports(),
                          super::parameters().desired_error(),
                          &train_mse, &train_bit_fail);
    cout << "training status = " << (train_ok ? "PASSED" : "FAILED") << endl;
    cout << "train data MSE = " << train_mse << endl;
    cout << "train data bit fail = " << train_bit_fail << endl;

    cout << "Testing ANN..." << endl;
    float test_mse;
    std::size_t test_bit_fail;
    bool test_ok = super::Test(test_data_out_file_path,
                               &test_mse, &test_bit_fail);
    cout << "test status = " << (test_ok ? "PASSED" : "FAILED") << endl;
    cout << "test data MSE = " << test_mse << endl;
    cout << "test data bit fail = " << test_bit_fail << endl;

    return train_ok && test_ok;
  }

};



}  // namespace ann

}  // namespace slesann

#endif /* IMAGE_MATCHING_TRAINER_H_ */
