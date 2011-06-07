/*
 * vararg-templatelist.cpp
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
 * vararg-templatelist.cpp
 *
 *  Created on: Apr 3, 2011
 *      Author: losvald
 */

#include "../test.h"

#ifndef SKIP_TESTS

#include <iostream>
#include <string>
#include <vector>


#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>

using namespace std;

//namespace bmpl = boost::mpl;
//template<class Typelist>
//class X : bmpl::inherit_linearly<Typelist, bmpl::inherit<bmpl::_1, bmpl::_2> >::type {
//
//  bmpl::at_c<Typelist, 3> a;
//  bmpl::size<Typelist> b;
//
//};


struct AbstractSubexpression {
};

class Nothing {};

template<class T1 = Nothing, class T2 = Nothing, class T3 = Nothing>
class Expression;

template<>
class Expression<Nothing, Nothing, Nothing> {};

template<class T1, class T2, class T3>
class Expression : public Expression<T2, T3> {

  void PrintAll() {

  }
};



struct Subexpression1 : public AbstractSubexpression {
  void operator()() const {
    cout << "s1" << endl;
  }
};

struct Subexpression2 : public AbstractSubexpression {
  void operator()() const {
    cout << "s2" << endl;
  }
};

struct Subexpression3 : public AbstractSubexpression {
  void operator()() const {
    cout << "s3" << endl;
  }
};

struct Subexpression4 : public AbstractSubexpression {
  void operator()() const {
    cout << "s4" << endl;
  }
};

template<class T>
struct Printer {
  T t;
  bool operator()() const {
    t();
    return true;
  }
};

//typedef X< boost::mpl::vector<Subexpression1, Subexpression2, Subexpression3> > MyX;
#if 0
int main(int argc, char **argv) {
  Expression<Subexpression1, Subexpression2, Subexpression3> a;

  Printer<Subexpression1> p;
  p();
  cout << "end" << endl;
  return 0;
}
#endif

#endif
