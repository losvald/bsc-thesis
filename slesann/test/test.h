/*
 * test.h
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
 * test.h
 *
 *  Created on: Mar 30, 2011
 *      Author: losvald
 */

#ifndef TEST_H_
#define TEST_H_

//#define SKIP_TESTS 1
#define VERBOSITY 1

#include <gtest/gtest.h>

template<class InputIterator1, class InputIterator2>
void ExpectEq(InputIterator1 exp_begin, InputIterator2 exp_end,
              InputIterator1 act_begin, InputIterator2 act_end) {
  InputIterator1 exp_it = exp_begin;
  InputIterator2 act_it = act_begin;
  for (; ; ++exp_it, ++act_it) {
    if (exp_it == exp_end && act_it == act_end)
      return true;
    if (exp_it == exp_end || act_it == act_end)
      return false;
    if (!(*exp_it == *act_it))
      return false;
  }
  return true;
}

#endif /* TEST_H_ */
