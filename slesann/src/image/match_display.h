/*
 * match_display.h
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
 * match_display.h
 *
 *  Created on: Jun 7, 2011
 *      Author: losvald
 */

#ifndef MATCH_DISPLAY_H_
#define MATCH_DISPLAY_H_

#include "image_base.h"

namespace slesann {

void ShowMatchingImages(const ImagePath& img_path1, const ImagePath& img_path2,
                        float scale_factor, bool wait_key = false);

}  // namespace slesann


#endif /* MATCH_DISPLAY_H_ */
