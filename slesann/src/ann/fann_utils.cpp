/*
 * fann_utils.cpp
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
 * fann_utils.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#include <fann.h>

#include "fann_utils.h"

namespace slesann {

namespace ann {

bool GetMseAndBitFail(fann* ann, float** mse, std::size_t** bit_fail) {
  float mse_tmp = fann_get_MSE(ann);
  std::size_t bit_fail_tmp = static_cast<std::size_t>(fann_get_bit_fail(ann));
  if (*mse != NULL)
    **mse = mse_tmp;
  if (*bit_fail != NULL)
    **bit_fail = bit_fail_tmp;
  return !bit_fail_tmp;
}

//void CreateTrainingFile(const boost::filesystem::path& file_path) {
//
//}

}  // namespace ann

}  // namespace slesann
