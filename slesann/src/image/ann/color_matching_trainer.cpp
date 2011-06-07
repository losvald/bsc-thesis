/*
 * color_matching_trainer.cpp
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
 * color_matching_trainer.cpp
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#include "color_matching_trainer.h"
#include "color_input_transformer.h"

namespace slesann {

namespace ann {

namespace {


}  // namespace

ColorMatchingTrainer::ColorMatchingTrainer(const super::Params& params)
: super(InputDataTransformerSharedPtr(new ColorInputTransformer()), params) {}

}  // namespace ann

}  // namespace slesann
