/*
 * ezlogger-example.cpp
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
 * ezlogger-example.cpp
 *
 *  Created on: Mar 31, 2011
 *      Author: losvald
 */

#define EZLOGGER_IMPLEMENT_DEBUGLOGGING

// include assert if DEBUG LOGGING is on (required by some functions)
#ifdef EZLOGGER_IMPLEMENT_DEBUGLOGGING
#include <cassert>
#endif

#include "../../src/ezlogger/ezlogger_headers.hpp"

void some_funcxx()
{
        EZLOGGERFUNCTRACKER;
}

void some_funcx()
{
        EZLOGGERFUNCTRACKER;
        some_funcxx();
}

void some_func6()
{
        EZLOGGERFUNCTRACKER;
        EZLOGGERDISPLAY_STACK;
}

void some_func5(int &x)
{
        EZLOGGERFUNCTRACKER;
        --x;
        if (x > 0)
                some_func5(x); //test recursion
        else
                some_func6();
}

void some_func4()
{
        EZLOGGERFUNCTRACKER;
        int x = 3;
        some_func5(x);
}

void some_func3()
{
        EZLOGGERFUNCTRACKER;
        some_funcx();
        some_funcxx();
        some_func4();
}

void some_func2()
{
        EZLOGGERFUNCTRACKER;
        some_funcx();
        some_funcxx();
        some_func3();
        some_funcxx();
}

void some_func1()
{
        EZLOGGERFUNCTRACKER;
        some_funcx();
        some_func2();
        some_funcxx();
}

void ezlogger_simple_example()
{
        int i = 123;
        std::string somedata = "Hello World";

        //Simple usage with standard verbosity level
        EZLOGGERSTREAM << somedata << " " << i << std::endl;

        //Can use alternate stream
        EZLOGGERSTREAM2(std::cerr) << somedata << " next line " << i << std::endl;


        EZLOGGERSTREAM2(std::cout) << "Second line" << std::endl;
        EZLOGGERSTREAM << "Third line" << std::endl;

        //Verbosity level logging example
        EZLOGGERVLSTREAM(axter::log_often) << somedata << " " << i << std::endl;

        //Complex extended data example
        EZLOGGERVLSTREAM(axter::levels(axter::log_often, axter::warn, __PRETTY_FUNCTION__, "Xyz Facility")) << somedata << " " << i << std::endl;
}

int ezlogger_complex_example(int argc, char **argv) {
  axter::ezlogger<>::set_verbosity_level_tolerance(axter::log_very_rarely);
         EZLOGGERFUNCTRACKER;
         int ReturnValue = 99;
         EZLOGGER_PRG_MAIN_ARG(argc, argv);
         EZDBGONLYLOGGER_PRG_MAIN_ARG(argc, argv);
         EZLOGGERVL_PRG_MAIN_ARG(axter::log_often, argc, argv);
         int i = 123;
         std::string somedata = "Hello World";
         EZLOGGER(i);
         EZDBGONLYLOGGER(i);
         EZLOGGERVL(axter::log_often)(i);

         EZLOGGERVAR(somedata);
         EZDBGONLYLOGGERVAR(somedata);
         EZLOGGERVLVAR(axter::log_often, somedata);

         bool SomeConditionVar = true;
         EZLOGGERVAR(SomeConditionVar == false);
         EZDBGONLYLOGGERVAR(SomeConditionVar == false);
         EZLOGGERVLVAR(axter::log_often, SomeConditionVar == true);

         EZLOGGERVLVARIFY(axter::log_often, SomeConditionVar == false);

         EZLOGGERSTREAM << somedata << " OVO JE PROBA " << std::endl;
         EZLOGGERSTREAM << somedata << " " << i << std::endl;
         EZLOGGERSTREAM << somedata << " next line " << i << std::endl;
         EZLOGGERSTREAM2(std::cerr) << somedata << " next line " << i << std::endl;
         EZDBGONLYLOGGERSTREAM << somedata << " " << i << std::endl;
         EZDBGONLYLOGGERSTREAM << somedata << " next line " << i << std::endl;
         EZLOGGERVLSTREAM(axter::log_often) << somedata << " " << i << std::endl;
         //EZLOGGERVLSTREAM(axter::levels(axter::log_often, axter::warn, __FUNCSIG__ /*or GNU PRETTY_FUNCTION*/,"Xyz Facility")) << somedata << " " << i << std::endl;

         EZLOGGERPRINT("i = %i and somedata = %s", i, somedata.c_str());
         EZDBGONLYLOGGERPRINT("i = %i and somedata = %s", i, somedata.c_str());
         EZLOGGERVLPRINT(axter::log_often)("i = %i and somedata = %s", i, somedata.c_str());
         //Alternative method
         EZLOGGERVL(axter::log_often).cprint("i = %i and somedata = %s", i, somedata.c_str());
         EZLOGGER.cprint("i = %i and somedata = %s", i, somedata.c_str());

         if (1)
         {
                 EZLOGGERMARKER;
                 EZDBGONLYLOGGERMARKER;
                 EZLOGGERVLMARKER(axter::log_often);
         }

         some_func1();

         return EZLOGGERVAR(ReturnValue);
}


#if 0
int main(int argc, char **argv) {
  //ezlogger_simple_example();

  EZLOGGER(5, 4, 3);
  EZLOGGERPRINT("%d + %d = %d", 2, 3, 5);


  EZDBGONLYLOGGERPRINT("%d + %d = %d", -1, -2, -3);

  EZLOGGERMARKER;

  using namespace std;

  bool SomeConditionVar = false;
  int x = 3;
  EZLOGGERVAR(x == 2);

  EZLOGGERPRINT("%d + %d = %d", 6, 7, 13);

  return 0;
}
#endif
