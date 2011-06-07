/*
 * vlp_match.cpp
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
 * vlp_match.cpp
 *
 *  Created on: May 12, 2011
 *      Author: losvald
 */

#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "../../ann/ann_vector_storage.h"
#include "../../ann/data_builder.h"
#include "../../ann/input_vector.h"
#include "../../ann/output_vector.h"
#include "../../base.h"
#include "../file_name_utils.h"
#include "../plate_utils.h"
#include "file_name_utils.h"
#include "plate_ann.h"
#include "vlp_match.h"

namespace slesann {

namespace ann {

typedef std::multimap<std::string, fs::path> PlateFileMap;
typedef std::vector<std::pair<std::string, fs::path> > PlateFileVector;

namespace {

const char* kTrainingDataExtension = ".input";

std::size_t LicenceNumberCount(const PlateFileVector& sorted_v,
                               const std::string& licence_number) {
  std::size_t count = 0;
  PlateFileVector::const_iterator it =
      std::lower_bound(sorted_v.begin(), sorted_v.end(),
                       make_pair(licence_number, fs::path()));
  for (PlateFileVector::const_iterator end = sorted_v.end();
      it != end && it->first == licence_number; ++it)
    ++count;
  return count;
}

std::set<fs::path> RandomNonmatching(const PlateFileVector& sorted_v,
                                     const std::string& licence_number,
                                     std::size_t count = 0) {
  static boost::mt19937 gen;
  boost::uniform_int<> dist(0, (int)sorted_v.size() - 1);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);

  if (count == 0)
    count = LicenceNumberCount(sorted_v, licence_number);

  std::set<fs::path> ret;
  while (ret.size() < count) {
    std::size_t ind;
    while (sorted_v[ind = die()].first == licence_number) {}
    ret.insert(sorted_v[ind].second);
  }
  return ret;
}

std::size_t ComputeInputVectorSize(const std::size_t vec_size_max) {
  return vec_size_max * 2;
}

InputVector<PlateVectorElement> CreateInputVector(
    const boost::filesystem::path& p1, const boost::filesystem::path& p2,
    const std::size_t vec_size_max) {
  InputVector<PlateVectorElement> v(ComputeInputVectorSize(vec_size_max));
  PlateVector pv1 = PlateToVector(p1), pv2 = PlateToVector(p2);
  std::copy(pv1.begin(), pv1.end(), v.begin());
  std::copy(pv2.begin(), pv2.end(), v.begin() + vec_size_max);
  return v;
}

void CreateTrainingFile(const boost::filesystem::path& dir,
                        const PlateFileMap& plate_file_map,
                        const std::size_t vec_size_max) {
  typedef std::multimap<std::string, fs::path>::const_iterator Iter;

  // create temporary training files
  const fs::path tmp_dir(dir / "training-tmp");
  AnnVectorStorage training_avs(tmp_dir);

  for (Iter it = plate_file_map.begin(), end = plate_file_map.end();
      it != end; ++it) {
    std::string basename = fs::basename(it->second);
    // convert image to vector and save it to a file
    training_avs.Save(PlateToVector(it->second),
                      basename + kTrainingDataExtension);
    EZLOGGERSTREAM2(std::cerr) << "Created ANN input vector for "
        << basename << ": " << basename << std::endl;
  }

  // use temporary training files to build data set for training
  DataBuilder db(ComputeInputVectorSize(vec_size_max), 1);

  PlateFileVector v(plate_file_map.begin(), plate_file_map.end());
  std::sort(v.begin(), v.end());

  OutputVector<int> kZero(1); kZero[0] = 0;
  OutputVector<int> kOne(1); kOne[0] = 1;
  Iter to, end = plate_file_map.end();
  for (Iter from = plate_file_map.begin(); from != end; from = to) {
    to = from;
    for (++to; to != end && from->first == to->first; ++to) {}
    for (Iter it = from; it != to; ++it) {
      const fs::path& path1 = from->second;
      BOOST_FOREACH(const fs::path& path2, RandomNonmatching(v, it->first)) {
        db.Append(CreateInputVector(path1, path2, vec_size_max), kZero);
        EZLOGGERSTREAM2(std::cerr) << "Added training data: " <<
            fs::basename(path1.string()) << " != " <<
            fs::basename(path2.string()) << std::endl;
      }

      for (Iter it2 = from; it2 != it; ++it2) {
        const fs::path& path2 = it2->second;
        db.Append(CreateInputVector(path1, path2, vec_size_max), kOne);
        EZLOGGERSTREAM2(std::cerr) << "Added training data: " <<
            fs::basename(path1.string()) << " == " <<
            fs::basename(path2.string()) << std::endl;
      }
    }
  }

  // write data set for training to a file
  const fs::path training_file_path(dir / "vlp_match_train.data");
  std::ofstream of;
  of.open(training_file_path.string().c_str());
  of << db;
  of.close();
}

void RemoveSingletons(PlateFileMap* plate_file_map) {
  typedef std::multimap<std::string, fs::path>::const_iterator Iter;
  std::vector<std::string> to_remove;
  Iter to, end = plate_file_map->end();
  for (Iter from = plate_file_map->begin(); from != end; from = to) {
    to = from;
    std::size_t count = 1;
    for (++to; to != end && from->first == to->first; ++to)
      ++count;
    if (count < 3) // XXX
      to_remove.push_back(from->first);
  }
  BOOST_FOREACH(const std::string& s, to_remove) {
    plate_file_map->erase(s);
  }
}

}  // namespace

void TrainLicencePlateMatching(const boost::filesystem::path& dir) {
  namespace fs = boost::filesystem;

  PlateFileMap plate_file_map;

  std::cout << "Ovo je proba: " << dir.string() << "\n";

  std::size_t vec_size_max = 0, vec_size_min = 0x3f3f3f3f, vec_size_avg = 0;
  std::size_t vec_count = 0;

  for (fs::directory_iterator it(dir), it_end; it != it_end; ++it) {
    if (fs::is_regular_file(it->status()) && IsPlate(it->path())) {

      plate_file_map.insert(std::make_pair(
          GetLicenceNumber(it->path()), it->path()));
    }
  }

  RemoveSingletons(&plate_file_map);

  for (PlateFileMap::const_iterator it = plate_file_map.begin(),
      end = plate_file_map.end(); it != end; ++it) {
    const fs::path& path = it->second;
    std::cout << "File: " << path << std::endl;
    std::cout << "Plate number: " << GetLicenceNumber(path) << std::endl;
    std::cout << "Checkpoint name: " << GetCheckpointName(path) << std::endl;

    std::size_t vec_size = VectorSize(path);
    if (vec_size > vec_size_max) vec_size_max = vec_size;
    if (vec_size < vec_size_min && vec_size > 100) vec_size_min = vec_size;
    if (vec_size == 0) {
      PrintError(EXIT_FAILURE, path.string());
    }
    vec_size_avg += vec_size;
    ++vec_count;
  }
  if (vec_count)
    vec_size_avg /= vec_count;

  EZLOGGERSTREAM2(std::cerr) << "Scanned " << vec_count << " licence plates" <<
      std::endl << "Max vector size: " << vec_size_max << std::endl <<
      "Min vector size: " << vec_size_min << std::endl <<
      "Avg vector size: " << vec_size_avg << std::endl;


  CreateTrainingFile(dir, plate_file_map, vec_size_max);
}

}  // namespace ann

}  // namespace slesann
