/*
 * plate_input_transformer.h
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
 * plate_input_transformer.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef PLATE_INPUT_TRANSFORMER_H_
#define PLATE_INPUT_TRANSFORMER_H_

#include "image_input_transformer.h"

namespace slesann {

namespace ann {

class PlateInputTransformer : public ImageInputTransformer {
public:
  PlateInputTransformer();
  InputVector<float> operator()(const ImagePath& data) const;
};

}  // namespace ann

}  // namespace slesann

#endif /* PLATE_INPUT_TRANSFORMER_H_ */
