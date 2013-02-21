/*
 *       TrainProfileHMMMaxLikelihood.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#ifndef TRAIN_MAX_LIKELIHOOD_HPP
#define TRAIN_MAX_LIKELIHOOD_HPP

#include "crossplatform.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <set>

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a ProfileHMM using MaxLikelihood
  class DLLEXPORT TrainProfileHMMMaxLikelihood : public ProbabilisticModelCreator {
  public:
    TrainProfileHMMMaxLikelihood () {}
    virtual ~TrainProfileHMMMaxLikelihood () {};
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual void infer_model(string fastafile, int pseudocounts, double residuefraction, string datetime, const vector<string>& symbols) const;
    virtual bool match_expression(string input, string er) const;

    virtual void model_file_creator(int n_match_states,const vector<string>& symbols, string model_filename) const;
    virtual void config_file_creator(string states_filename, string emiss_filename, string model_filename, string config_filename, int pseudocounts) const;
    virtual void emiss_states_file_creator(const vector<string>& seq, int index[], bool match[], const vector<string>& seq_names, string emiss_filename, string states_filename) const;


    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }


  };
  typedef boost::shared_ptr <TrainProfileHMMMaxLikelihood> TrainProfileHMMMaxLikelihoodPtr ;
};


#endif
