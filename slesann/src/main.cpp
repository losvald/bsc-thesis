/*
 * main.cpp
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
 * main.cpp
 *
 *  Created on: Mar 30, 2011
 *      Author: losvald
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "units.h"

#include "base.h"

#include "vehicle_descriptor_provider.h"
#include "processing_loop.h"

// dummy
#include "dummy-noimage/dummy_checkpoint.h"
#include "dummy-noimage/dummy_descriptor_provider.h"


namespace slesann {

typedef std::string Filename;

bool CheckInputFile(const Filename& f) {
  namespace fs = boost::filesystem;
  fs::path p(f);
  // TODO

  return f != "";
}

std::pair<std::string, std::string> ParseVerbosity(const std::string& s) {
  if (s.find("-v") == 0) {
    size_t value = 1;
    try {
      value = boost::lexical_cast<size_t>(s.substr(2));
    }
    catch(...) {
      while (s[1 + value] == 'v')
        ++value;
    }
    return std::make_pair("verbose", boost::lexical_cast<std::string>(value));
  }
  return std::make_pair(std::string(), std::string());
}

void ParseOptions(int argc, char **argv) {
  namespace po = boost::program_options;

  po::options_description desc("Available options");
  desc.add_options()
        ("help,h", "prints help")
        ("verbose,v", po::value< std::vector<std::string> >(), "increases verbosity level (more detailed output)")
        ("dummy-start,s", po::value <std::string>(), "start checkpoint filename")
        ("dummy-end,e", po::value <std::string>(), "end checkpoint filename")
        //    ("action,a"           , po::value<string>(&action), "desired program action (generate | solve | gensolve ) ")
        //
        //    ("currencies-file,c"  , po::value<filename>(&currencies)              , "currencies input file")
        //    ("purchasables-file,p", po::value<filename>(&purchasables)            , "purchasables input file")
        //    ("supplies-file,u"    , po::value<filename>(&supplies)                , "supplies input file")
        //    ("sellers-file,s"     , po::value<filename>(&sellers)                 , "sellers input file")
        //    ("requests-file,r"    , po::value<filename>(&requests)                , "requests input file")
        //    ("offers-file,o"    , po::value<filename>(&offers)                  , "offers input file")
        //    ("ga-param-file,g"    , po::value<filename>(&ga_param)                , "ga param input file")
        //
        //    ("price-ranges-file,P", po::value<filename>(&priceRanges)             , "price ranges output file if generating")
        //    ("seed,S"               , po::value<int>(&seed)                       , "seed number")
        //    ("num-offers,n"       , po::value<int>(&numOffers)                    , "number of offers")
        //    ("supply-min,m"       , po::value<int>(&supplyMin)                    , "supply minimum value")
        //    ("supply-max,M"       , po::value<int>(&supplyMax)                    , "supply maximum value")
        //
        //    ("algorithm,A"        , po::value<string>(&algo)                      , "algorithm to use for solving")
        ;

  po::variables_map var_map;

  try {
    po::store(po::parse_command_line(argc, argv, desc, 0, ParseVerbosity), var_map);
  }
  catch (po::error e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  po::notify(var_map);

  if(var_map.count("help") ){
    std::cerr << desc;
    exit(EXIT_SUCCESS);
  }

  // set verbosity level
  std::size_t verbosity = 0;
  if (var_map.count("verbose")) {
    BOOST_FOREACH(const std::string& s, var_map["verbose"].as<std::vector<std::string> >()) {
      verbosity += boost::lexical_cast<int>(s);
    }
    switch (verbosity) {
      case '0': // no verbosity
        break;
      case '1':
        environment::verbosity = axter::log_often;
        break;
      default:
        environment::verbosity = axter::log_always;
        break;
    }

    EZLOGGERVLSTREAM2(axter::levels(axter::log_often, axter::info), std::cout) <<
            "Set verbosity to " << verbosity;
  }

}


namespace { // TEST

SpeedLimitEnforcer<DummyVehicleDescriptor>* CreateVehicleImageProvider(
    const std::string& start_filename,
    const std::string& end_filename) {
  DummyVehicleDescriptorProvider start_vip;
  std::ifstream start_in;
  start_in.open(start_filename.c_str());
  start_in >> start_vip;
  start_in.close();

  DummyVehicleDescriptorProvider end_vip;
  std::ifstream end_in;
  end_in.open(end_filename.c_str());
  end_in >> end_vip;
  end_in.close();

  units::Distance distance = 100. * boost::units::si::meter;
  units::Speed min_speed = 10. * boost::units::si::metres_per_second;
  units::Speed max_speed = 100. * boost::units::si::metres_per_second;
  return new SpeedLimitEnforcer<DummyVehicleDescriptor>(0, 1,
                                distance,
                                min_speed,
                                max_speed);
}

} // namespace


} // namespace slesann

#if 1
int main(int argc, char **argv) {
  using namespace slesann;

  ParseOptions(argc, argv);
  environment::InitLogger();


  ProcessingLoop<DummyVehicleDescriptor> loop(CreateVehicleImageProvider(
      "c1.txt",
      "c2.txt"
  ));


  EZDBGONLYLOGGERSTREAM2(std::cerr) << "EXITING" << std::endl;
  EZDBGONLYLOGGER("Quit");
  return EXIT_SUCCESS;
}
#endif
