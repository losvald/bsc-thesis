/*
 * matching_trainer.h
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
 * matching_trainer.h
 *
 *  Created on: May 17, 2011
 *      Author: losvald
 */

#ifndef MATCHING_TRAINER_H_
#define MATCHING_TRAINER_H_

#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include <boost/assert.hpp>
#include <boost/filesystem/path.hpp>

#include "ann.h"
#include "ann_data_transformation.h"
#include "data_builder.h"
#include "trainer.h"
#include "training_parameters.h"

namespace slesann {

namespace ann {

//class TrainingParameters;

//class MatchingTrainerParameters {
//public:
//  MatchingTrainerParameters(const TrainingParameters& training_params);
//  MatchingTrainerParameters(
//      const std::vector<std::size_t>& hidden_layer_neuron_counts,
//      const float* random_weight_limit,
//      std::size_t max_epochs,
//      std::size_t epochs_between_reports,
//      float desired_error,
//      float bit_fail_limit,
//      const TrainingAlgorithm* training_algo,
//      float learning_rate,
//      float learning_momentum);
//
//  const std::vector<std::size_t>& hidden_neuron_counts() const;
//  const float* random_weight_limit() const;
//  std::size_t max_epochs() const;
//  std::size_t epochs_between_reports() const;
//  float desired_error() const;
//  float bit_fail_limit() const;
//  const TrainingAlgorithm& training_algo() const;
//  float learning_rate() const;
//  float learning_momentum() const;
//
//private:
//  std::vector<std::size_t> hidden_neuron_counts_;
//  const float* random_weight_limit_;
//  std::size_t max_epochs_;
//  std::size_t epochs_between_reports_;
//  float desired_error_;
//  float bit_fail_limit_;
//  const TrainingAlgorithm* training_algo_;
//  float learning_rate_;
//  float learning_momentum_;
//};

template<typename T, typename U, bool cacheTransformations = true>
class MatchingTrainer : public Trainer {
public:
  typedef std::pair<T, T> AnnInputPair;
  typedef std::map<AnnInputPair, U> AnnIOMap;
  typedef std::pair<AnnInputPair, U> AnnIOPair;
  typedef InputDataTransformer<T, float> InputDataTransformerType;
  typedef OutputDataTransformer<U, float> OutputDataTransformerType;
  typedef typename InputDataTransformerType::SharedPtr
      InputDataTransformerSharedPtr;
  typedef typename OutputDataTransformerType::SharedPtr
      OutputDataTransformerSharedPtr;
  typedef std::pair<InputVector<float>, OutputVector<float> > AnnMapping;

  typedef TrainingParameters Params;

protected:

  static std::ostream& PrintAnnMappings(std::ostream& os,
                                        const AnnIOMap& io_map) {
    for (typename AnnIOMap::const_iterator it = io_map.begin(),
        end = io_map.end(); it != end; ++it) {
      os << it->first.first << ", " << it->first.second << " -> " <<
          it->second << std::endl;
    }
    return os;
  }

private:
  void SaveData(const boost::filesystem::path& file_path,
                const AnnIOMap& io_map) const {
    DataBuilder db(2 * input_trans_->input_neuron_count(),
                   output_trans_->output_neuron_count());
    for (typename AnnIOMap::const_iterator it = io_map.begin(),
        end = io_map.end(); it != end; ++it) {
      db.Append(TransformInputPair(it->first), (*output_trans_)(it->second));
    }
    db.Save(file_path);
  }

  std::vector<AnnMapping> GetAnnMappings(const AnnIOMap& io_map) const {
    std::vector<AnnMapping> ret;
    ret.reserve(io_map.size());
    for (typename AnnIOMap::const_iterator it = io_map.begin(),
        end = io_map.end(); it != end; ++it) {
      ret.push_back(AnnMapping(TransformInputPair(it->first),
                               (*output_trans_)(it->second)));
    }
    return ret;
  }

  bool GetStatistics(const AnnIOMap& io_map,
                     std::vector<AnnIOPair>* failed = NULL,
                     float* fail_percentage = NULL,
                     float* mse = NULL,
                     std::size_t* bit_fail = NULL,
                     std::vector<float>* square_errors = NULL) {
    if (failed != NULL) failed->clear();
    if (square_errors != NULL) square_errors->clear();

    float total_mse = 0;
    std::size_t total_bit_fail = 0;
    std::size_t total_count = io_map.size(), failed_count = 0;
    for (typename AnnIOMap::const_iterator it = io_map.begin(),
        end = io_map.end(); it != end; ++it) {
      float square_error; std::size_t cur_bit_fail;
      bool passed = Test(it->first.first, it->first.second, it->second,
                         &square_error, &cur_bit_fail);
      if (!passed) {
        ++failed_count;
        if (failed != NULL) failed->push_back(*it);
        if (square_errors != NULL) square_errors->push_back(square_error);
      }
      total_mse += square_error;
      total_bit_fail += cur_bit_fail;
    }
    if (total_count != 0)
      total_mse /= total_count;

    if (fail_percentage != NULL)
      *fail_percentage = static_cast<float>(failed_count) / total_count;
    if (mse != NULL)
      *mse = total_mse;
    if (bit_fail != NULL)
      *bit_fail = total_bit_fail;

    return failed_count == 0;
  }

public:

  MatchingTrainer(InputDataTransformerSharedPtr input_data_transformer,
                  OutputDataTransformerSharedPtr output_data_transformer,
                  const Params& params)
  : ann_(input_data_transformer->input_neuron_count(),
         output_data_transformer->output_neuron_count(),
         params.hidden_neuron_counts()),
    input_trans_(input_data_transformer),
    output_trans_(output_data_transformer),
    params_(params) {
    set_ann(ann_.ann());
  }

  friend std::ostream& operator<<(std::ostream& os, const MatchingTrainer& t) {
    os << "Train data:" << std::endl;
    PrintAnnMappings(os, t.train_io_map_);
    os << "Test data:" << std::endl;
    PrintAnnMappings(os, t.test_io_map_);
    return os;
  }

  bool AddForTrain(const AnnInputPair& input_pair, const U& output) {
    return train_io_map_.insert(std::make_pair(input_pair, output)).second;
  }

  bool AddForTrain(const T& input1, const T& input2, const U& output) {
    return AddForTrain(AnnInputPair(input1, input2), output);
  }

  bool AddForTest(const AnnInputPair& input_pair, const U& output) {
    return test_io_map_.insert(std::make_pair(input_pair, output)).second;
  }

  bool AddForTest(const T& input1, const T& input2, const U& output) {
    return AddForTest(AnnInputPair(input1, input2), output);
  }

  virtual void ClearTransformationCache() {
    if (cacheTransformations) {
      input_transformation_memo_.clear();
      output_transformation_memo_.clear();
    }
  }

  void CreateTrainingFile(const boost::filesystem::path& file_path) const {
    SaveData(file_path, train_io_map_);
  }

  void CreateTestFile(const boost::filesystem::path& file_path) const {
    SaveData(file_path, test_io_map_);
  }

  bool Test(const T& input1, const T& input2, const U& output,
            float* square_error = NULL, std::size_t* bit_fail = NULL) {
    return Test(TransformInputPair(AnnInputPair(input1, input2)),
                TransformOutput(output),
                square_error, bit_fail);
  }

  bool GetTrainDataStatistics(std::vector<AnnIOPair>* failed = NULL,
                              float* fail_percentage = NULL,
                              float* mse = NULL,
                              std::size_t* bit_fail = NULL,
                              std::vector<float>* square_errors = NULL) {
    return GetStatistics(train_io_map_, failed, fail_percentage, mse, bit_fail,
                         square_errors);
  }

  bool GetTestDataStatistics(std::vector<AnnIOPair>* failed = NULL,
                             float* fail_percentage = NULL,
                             float* mse = NULL,
                             std::size_t* bit_fail = NULL,
                             std::vector<float>* square_errors = NULL) {
    return GetStatistics(test_io_map_, failed, fail_percentage, mse, bit_fail,
                         square_errors);
  }

  bool Train(const boost::filesystem::path& train_data_out_file_path,
             const float* random_weight_limit,
             std::size_t max_epochs, std::size_t epochs_between_reports,
             float desired_error, float* mse = NULL,
             std::size_t* bit_fail = NULL) {
    CreateTrainingFile(train_data_out_file_path);
    BOOST_ASSERT(ann_.ann() == Trainer::ann());
    return Trainer::Train(AnnData(train_data_out_file_path),
                          random_weight_limit, max_epochs,
                          epochs_between_reports, desired_error, mse, bit_fail);
  }

  bool Test(const boost::filesystem::path& test_data_out_file_path,
            float* mse = NULL, std::size_t* bit_fail = NULL) {
    CreateTestFile(test_data_out_file_path);
    return Trainer::Test(AnnData(test_data_out_file_path), mse, bit_fail);
  }

  const Ann& ann() const {
    return ann_;
  }

  const InputDataTransformerType& input_data_transformer() const {
    return *input_trans_;
  }

  const OutputDataTransformerType& output_data_transformer() const {
    return *output_trans_;
  }

  const Params parameters() const {
    return params_;
  }

  virtual std::vector<AnnMapping> ann_train_mappings() const {
    return GetAnnMappings(train_io_map_);
  }

  virtual std::vector<AnnMapping> ann_test_mappings() const {
    return GetAnnMappings(test_io_map_);
  }

protected:
  InputVector<float> TransformInputPair(const AnnInputPair& input_pair) const {
    return MergeInputVectors(TransformInput(input_pair.first),
                             TransformInput(input_pair.second));
  }

  virtual InputVector<float> TransformInput(const T& input_data) const {
    if (cacheTransformations) {
      if (input_transformation_memo_.count(input_data))
        return input_transformation_memo_[input_data];
      return input_transformation_memo_[input_data] =
          (*input_trans_)(input_data);
    }
    return (*input_trans_)(input_data);
  }

  virtual OutputVector<float> TransformOutput(const U& output_data) const {
    if (cacheTransformations) {
      if (output_transformation_memo_.count(output_data))
        return output_transformation_memo_[output_data];
      return output_transformation_memo_[output_data] =
          (*output_trans_)(output_data);
    }
    return (*output_trans_)(output_data);
  }

private:
  Ann ann_;
  InputDataTransformerSharedPtr input_trans_;
  OutputDataTransformerSharedPtr output_trans_;
  AnnIOMap train_io_map_;
  AnnIOMap test_io_map_;
  mutable std::map<T, InputVector<float> > input_transformation_memo_;
  mutable std::map<U, OutputVector<float> > output_transformation_memo_;
  Params params_;

  InputVector<float> MergeInputVectors(const InputVector<float>& v1,
                                       const InputVector<float>& v2) const {
    InputVector<float> merged(v1.size() + v2.size());
    std::copy(v1.begin(), v1.end(), merged.begin());
    std::copy(v2.begin(), v2.end(), merged.begin() + v1.size());
    return merged;
  }
};


}  // namespace ann

}  // namespace slesann

#endif /* MATCHING_TRAINER_H_ */
