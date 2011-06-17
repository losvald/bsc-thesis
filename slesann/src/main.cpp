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
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>


#include "units.h"

#include "base.h"
#include "file_utils.h"

#include "processing_loop.h"

#include "ann/training_algorithm.h"
#include "ann/training_parameters.h"

#include "image/ann/vlp_match.h"
#include "image/ann/plate_ann.h"
#include "image/color_hasher.h"
#include "image/default_vehicle_matcher.h"
#include "image/matcher_io_utils.h"

#include "image/ann/discriminator_training_io.h"
#include "image/ann/color_matching_trainer.h"
#include "image/ann/plate_matching_trainer.h"
#include "image/ann/shape_matching_trainer.h"

#include "../test/ann/custom_ann_test.h"

#include "../test/image/boundary_test.h"
#include "../test/image/match_display_test.h"
#include "../test/image/color_discrimination_test.h"
#include "../test/image/shape_discrimination_test.h"
#include "../test/image/plate_discriminator_test.h"

#include "../test/image/ann/shape_matching_trainer_test.h"

// dummy
//#define DUMMY_ENABLED // comment out to disable dummy
#ifdef DUMMY_ENABLED
#include "dummy-noimage/dummy_processing_loop_factory.h"
#endif

namespace slesann {

struct GlobalArguments {
  double checkpoint_distance;
  double min_speed;
  double max_speed;
} g_arguments;

#ifdef DUMMY_ENABLED
struct DummyGlobalArguments {
  std::string start_cp_filename;
  std::string end_cp_filename;
  std::string limits_filename;
} g_dummy_arguments;
#endif




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

boost::program_options::variable_value GetRequiredOption(
    boost::program_options::variables_map& var_map,
    const std::string& option_name) {
  if (!var_map.count(option_name))
    PrintError(EXIT_FAILURE, "Missing required option: " + option_name);
  return var_map[option_name];
}

void ParseOptions(int argc, char **argv) {
  namespace po = boost::program_options;

  Filename ann_out_filename;

  po::options_description desc("Available options");
  desc.add_options()
        ("help,h", "prints help")
        ("verbose,v", po::value< std::vector<std::string> >(),
            "increases verbosity level (more detailed output)")
        ("train-vlp-match", po::value<std::string>(),
            "trains Vehicle Licence Plate Matching")
        ("load-plate", po::value<std::string>(),
            "loads plate image")
        ("run-matcher", po::value<FilenameList>()->multitoken(),
            "runs the matching algorithm on extracted images")
        ("find-body-hull", po::value<Filename>(),
            "find the hull of the vehicle body")
        ("color-test", po::value<Filename>(),
            "tests color discrimination")
        ("plate-test", po::value<Filename>(),
            "tests plate discrimination")
        ("shape-stat", po::value<Filename>(),
            "computes shape discrimination statistics")
        ("color-stat", po::value<Filename>(),
            "computes color discrimination statistics")
        ("plate-stat", po::value<Filename>(),
            "computes plate discrimination statistics")
        ("train-config", po::value<Filename>(),
            "configured training parameters from the specified file")
        ("train-algo,a", po::value<std::string>()->default_value("rprop"),
            "sets the algorithm for ANN training")
        ("max-epochs,x", po::value<std::size_t>()->default_value(1000),
            "sets the maximum number of epoch for ANN training")
        ("epochs-per-report,r", po::value<std::size_t>()->default_value(10),
            "sets the number of epochs between reports for ANN training")
        ("stop-mse,e", po::value<float>()->default_value(0.f),
            "sets the Mean Square Error for ANN training")
        ("bit-fail-limit,f", po::value<float>()->default_value(0.35f))
        ("learning-rate,l", po::value<float>()->default_value(0.7f),
            "sets the learning rate for ANN training")
        ("learning-momentum,m", po::value<float>()->default_value(0.f),
            "sets the learning momentum for ANN training")
        ("hidden-neurons,d", po::value<std::vector<std::size_t> >()->multitoken(),
            "sets hidden neuron counts for ANN training")
        ("input-neurons,i", po::value<std::size_t>(),
            "sets input neuron counts for ANN training")
        ("output-neurons,o", po::value<std::size_t>(),
             "sets output neuron counts for ANN training")
        ("randomize-weights,w", po::value<float>(),
            "initializes the weights to random range [-value, value]")
        ("save-ann", po::value<Filename>(&ann_out_filename),
            "saves trained ANN to a file")
        ("train-data", po::value<Filename>(),
            "sets the file to load train data used for ANN training")
        ("test-data", po::value<Filename>(),
            "sets the file to load test data used for ANN training")
        ("train-data-out", po::value<Filename>(),
            "sets the file to save train data used for ANN training")
        ("test-data-out", po::value<Filename>(),
            "sets the file to save test data used for ANN training")
        ("skip-data-gen,s",
            po::value<bool>()->default_value(false)->implicit_value(true),
            "skips the generation o train and test data out files")
        ("shape-train", "trains shape discrimination")
        ("color-train", "trains color discrimination")
        ("plate-train", "trains plate discrimination")
        ("custom-ann", po::value<FilenameList>()->multitoken(),
            "trains and tests custom neural network")
        ("test-show-match-images", po::value<Filename>(), "")
#ifdef DUMMY_ENABLED
        ("dummy-start", po::value<std::string>(
            &g_dummy_arguments.start_cp_filename), "start checkpoint filename")
        ("dummy-end", po::value<std::string>(
            &g_dummy_arguments.end_cp_filename), "end checkpoint filename")
        ("dummy-limits", po::value<std::string>(
            &g_dummy_arguments.limits_filename), "limits filename")
#endif
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

  if (var_map.count("train-vlp-match")) {
    boost::filesystem::path dir = AsDirectory(
        var_map["train-vlp-match"].as<std::string>());
    RequireExists(dir);
    slesann::ann::TrainLicencePlateMatching(dir);
  }

  if (var_map.count("load-plate")) {
    ann::PlateToVector(AsInputFile(var_map["load-plate"].as<std::string>()));
  }

  if (var_map.count("run-matcher")) {
    FilenameList filenames = var_map["run-matcher"].as<FilenameList>();
    if (filenames.size() != 2)
      PrintError(EXIT_FAILURE, "run-matcher option must take 2 file names");
    std::vector<FrameInfo> v1 = ReadVehiclesToMatch(AsInputFile(filenames[0]));
    std::vector<FrameInfo> v2 = ReadVehiclesToMatch(AsInputFile(filenames[1]));
    DefaultVehicleMatcher m;
    m.Match(v1, v2);
  }

  if (var_map.count("find-body-hull")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["find-body-hull"].as<Filename>());
    test::FindAndShowGreatestHull(file_path);
  }

  if (var_map.count("test-show-match-images")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["test-show-match-images"].as<Filename>());
    test::ShowMatchImages(file_path);
  }

  if (var_map.count("color-test")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["color-test"].as<Filename>());
    test::RunColorDetection(file_path);
  }

  if (var_map.count("plate-test")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["plate-test"].as<Filename>());
    test::TestPlateFilters(file_path);
  }

  if (var_map.count("shape-stat")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["shape-stat"].as<Filename>());
    test::ComputeShapeDiscriminationStatistics(file_path);
  }

  if (var_map.count("color-stat")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["color-stat"].as<Filename>());
    test::ComputeColorDiscriminationStatistics(file_path);
  }

  if (var_map.count("plate-stat")) {
    boost::filesystem::path file_path = AsInputFile(
        var_map["plate-stat"].as<Filename>());
    test::ComputePlateDiscriminationStatistics(file_path);
  }

  bool training = var_map.count("shape-train") | var_map.count("color-train") |
      var_map.count("plate-train") | var_map.count("custom-ann");

  if (training) {
    float random_weight_limit_val;
    bool random_weight_limit = var_map.count("randomize-weights");
    if (random_weight_limit)
      random_weight_limit_val = var_map["randomize-weights"].as<float>();

    const ann::TrainingAlgorithm* training_algorithm =
        ann::TrainingAlgorithm::Get(var_map["train-algo"].as<std::string>());
    if (training_algorithm == NULL)
      PrintError(EXIT_FAILURE, "unknown training algorithm: " +
                 var_map["train-algo"].as<std::string>());

    ann::TrainingParameters train_params(
        GetRequiredOption(var_map, "hidden-neurons").as<std::vector<std::size_t> >(),
        (random_weight_limit ? &random_weight_limit_val : NULL),
        var_map["max-epochs"].as<std::size_t>(),
        var_map["epochs-per-report"].as<std::size_t>(),
        var_map["stop-mse"].as<float>(),
        var_map["bit-fail-limit"].as<float>(),
        training_algorithm,
        var_map["learning-rate"].as<float>(),
        var_map["learning-momentum"].as<float>()
    );

    boost::filesystem::path ann_out_file_path;
    if (!ann_out_filename.empty())
      ann_out_file_path = AsOutputFile(ann_out_filename, true);

    ann::ImageMatchingTrainer<true>* trainer = NULL;
    if (var_map.count("shape-train")) {
      std::cerr << "Creating shape matching trainer...\n" << std::endl;
      trainer = new ann::ShapeMatchingTrainer(train_params);
    } else if (var_map.count("color-train")) {
      trainer = new ann::ColorMatchingTrainer(train_params);
    } else if (var_map.count("plate-train")) {
      trainer = new ann::PlateMatchingTrainer(train_params);
    }

    std::size_t input_neuron_count, output_neuron_count;
    if (trainer != NULL) {
      input_neuron_count = trainer->ann().input_neuron_count();
      output_neuron_count = trainer->ann().output_neuron_count();
    } else {
      input_neuron_count = GetRequiredOption(var_map, "input-neurons")
          .as<std::size_t>();
      output_neuron_count = GetRequiredOption(var_map, "output-neurons")
              .as<std::size_t>();
    }

    if (var_map.count("custom-ann")) {
      FilenameList filenames = var_map["custom-ann"].as<FilenameList>();
      std::cerr << "filenames(" << filenames.size() << ") = [" <<
      filenames[0] << ", " << filenames[1] << "]" << std::endl;
      boost::filesystem::path train_data_file_path = AsInputFile(filenames[0]);
      boost::filesystem::path test_data_file_path = AsInputFile(filenames[1]);
      ann::test::TestCustomAnn(train_data_file_path, test_data_file_path,
                               input_neuron_count, output_neuron_count,
                               train_params);
      return ;
    }

    boost::filesystem::path train_data_file_path = AsInputFile(
        GetRequiredOption(var_map, "train-data").as<Filename>());
    boost::filesystem::path test_data_file_path = AsInputFile(
            GetRequiredOption(var_map, "test-data").as<Filename>());

    boost::filesystem::path train_data_out_file_path = AsOutputFile(
        GetRequiredOption(var_map, "train-data-out").as<Filename>(), true);
    boost::filesystem::path test_data_out_file_path = AsOutputFile(
        GetRequiredOption(var_map, "test-data-out").as<Filename>(), true);

    //      ann::test::TestLoadTrainingSet(train_data_file_path,
    //                                     test_data_file_path,
    //                                     train_data_out_file_path,
    //                                     test_data_out_file_path,
    //                                     train_params);

    if (trainer != NULL) {
      bool skip_gen = var_map["skip-data-gen"].as<bool>();
      if (!skip_gen) {
        std::cout << "Loading training data..." << std::endl;
        ann::LoadTrainDataSet(train_data_file_path, trainer);
        trainer->CreateTrainingFile(train_data_out_file_path);
        std::cout << "Loading test data..." << std::endl;
        ann::LoadTestDataSet(test_data_file_path, trainer);
        trainer->CreateTestFile(test_data_out_file_path);
      }

      std::cerr << "TrainAndTest shape matching trainer...\n" << std::endl;
      trainer->TrainAndTest(train_data_out_file_path,
                            test_data_out_file_path);
      if (!ann_out_file_path.empty()) {
        trainer->ann().Save(ann_out_file_path);
        EZLOGGERSTREAM2(std::cerr) << "Saved ANN to: " << ann_out_file_path <<
            std::endl;
      }
      delete trainer;
    }
  }

}

#ifdef DUMMY_ENABLED
void CheckDummyInputFiles() {
  namespace fs = boost::filesystem;
  std::string filenames[] = {g_dummy_arguments.start_cp_filename,
                             g_dummy_arguments.end_cp_filename,
                             g_dummy_arguments.limits_filename};
  BOOST_FOREACH(const std::string& filename, filenames) {
    fs::path p(filename);
    if (!fs::exists(p))
      PrintError(EXIT_FAILURE, "Filename \"" + p.string() + "\" does not exist");
    if (fs::is_directory(filename))
      PrintError(EXIT_FAILURE, "\"" + p.string() + "\" is a directory");
  }
}
#endif

} // namespace slesann

#ifndef RUN_TESTS_ENABLED
int main(int argc, char **argv) {
  using namespace slesann;

  ParseOptions(argc, argv);
  environment::InitLogger();

  EZLOGGERSTREAM2(std::cerr) << "Done parsing" << std::endl;

#ifdef DUMMY_ENABLED
  CheckDummyInputFiles();
//  BOOST_AUTO(loop, factory::CreateDummyProcessingLoop("01.start", "01.end"));
  BOOST_AUTO(loop, factory::CreateDummyProcessingLoop(
      g_dummy_arguments.start_cp_filename,
      g_dummy_arguments.end_cp_filename,
      g_dummy_arguments.limits_filename
      ));
  EZLOGGERSTREAM2(std::cerr) << "Running processing loop..";
  loop->run();
#endif

  EZDBGONLYLOGGERSTREAM2(std::cerr) << "EXITING" << std::endl;
  EZDBGONLYLOGGER("Quit");
  return EXIT_SUCCESS;
}
#endif
