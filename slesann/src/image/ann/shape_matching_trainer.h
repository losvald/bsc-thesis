/*
 * shape_matching_trainer.h
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
 * shape_matching_trainer.h
 *
 *  Created on: Jun 5, 2011
 *      Author: losvald
 */

#ifndef SHAPE_MATCHING_TRAINER_H_
#define SHAPE_MATCHING_TRAINER_H_

#include "image_matching_trainer.h"

namespace slesann {

namespace ann {

class ShapeMatchingTrainer : public ImageMatchingTrainer<true> {
  typedef ImageMatchingTrainer<true> super;
public:
  ShapeMatchingTrainer(const super::Params& params);
};

}  // namespace ann

}  // namespace slesann

#endif /* SHAPE_MATCHING_TRAINER_H_ */
