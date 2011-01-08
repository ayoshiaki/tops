/*
 *       ProbabilisticModelCreator.hpp
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

#ifndef PROBABILISTIC_MODEL_CREATOR_HPP
#define PROBABILISTIC_MODEL_CREATOR_HPP

#include "ProbabilisticModel.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Represents an algorithm to create a probabilistic model
  class ProbabilisticModelCreator {
  public:
    ProbabilisticModelCreator () {}
    virtual ~ProbabilisticModelCreator () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const {
      ProbabilisticModelPtr n;
      not_implemented( "create ()");
      return n;
    }
    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const {
      ProbabilisticModelPtr n;
      not_implemented( "create (parameters, loglikelihood)");
      return n;
    }

    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & sample_set, double & loglikelihood, int & sample_size) const {
      ProbabilisticModelPtr n;
      not_implemented( "create (parameters, loglikelihood)");
      return n;
    }

    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
      return create(parameters);
    }


    //! Provides a help
    virtual std::string help() const {
      std::string s;
      not_implemented( "help()");
      return s;
    }
    virtual void not_implemented( std::string method) const {
      std::cerr << "ERROR: Not implemented: " << method << " !" << std::endl;
      exit(-1);
    }
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      not_implemented("setCreator()");
      exit(-1);
    }


  };
  typedef boost::shared_ptr <ProbabilisticModelCreator> ProbabilisticModelCreatorPtr ;
};


#endif
