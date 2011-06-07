/*
 * plate_discriminator_test.h
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
 * plate_discriminator_test.h
 *
 *  Created on: May 30, 2011
 *      Author: losvald
 */

#ifndef PLATE_DISCRIMINATOR_TEST_H_
#define PLATE_DISCRIMINATOR_TEST_H_

#include <boost/filesystem/path.hpp>

namespace slesann {

namespace test {

void ComputePlateDiscriminationStatistics(
    const boost::filesystem::path& file_path);

void TestPlateFilters(const boost::filesystem::path& file_path);

}  // namespace test

}  // namespace slesann

#endif /* PLATE_DISCRIMINATOR_TEST_H_ */
