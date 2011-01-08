/*
 *       TrainWeightArrayModel.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#ifndef TRAIN_WAM_HPP
#define TRAIN_WAM_HPP

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! A creator that trains a Weight Array Model
  class TrainWeightArrayModel : public ProbabilisticModelCreator {
  public:
    TrainWeightArrayModel () {}
    virtual ~TrainWeightArrayModel () {};
    //! Creates a probability model given the parameters
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Creates a probability model given the parameters and returns the log likelihood of the training set.
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const ;
    //! Provides a help message 
    virtual std::string help() const {
      std::string s;
      return s;
    }

  };
  typedef boost::shared_ptr <TrainWeightArrayModel> TrainWeightArrayModelPtr ;
};


#endif
