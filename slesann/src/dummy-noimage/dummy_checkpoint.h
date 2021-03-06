/*
 * dummy_checkpoint.h
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
 * dummy_checkpoint.h
 *
 *  Created on: Apr 4, 2011
 *      Author: losvald
 */

#ifndef DUMMY_CHECKPOINT_H_
#define DUMMY_CHECKPOINT_H_

#include "../checkpoint.h"
#include "dummy_base.h"

namespace slesann {

typedef Checkpoint<DummyVehicleDescriptor> DummyCheckpoint;

}  // namespace slesann

#endif /* DUMMY_CHECKPOINT_H_ */
