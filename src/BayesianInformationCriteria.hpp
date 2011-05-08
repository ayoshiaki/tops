/*
 *       BayesianInformationCriteria.hpp
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

#ifndef BAYESIAN_INFORMATION_CRITERIA_HPP
#define BAYESIAN_INFORMATION_CRITERIA_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"

namespace tops
{
  //! Bayesian Information Criteria
  class DLLEXPORT BayesianInformationCriteria : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    BayesianInformationCriteria (){}
    BayesianInformationCriteria (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~BayesianInformationCriteria () {};
    //! Creates a new model using the received parameters
    /*! 
      \param parameters of the model's creators
      \return An instance of ProbabilisticModel
    */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;

    //! Returns a help message of this creator
    virtual std::string help() const;

    //! Set a creator 
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }

  };
  typedef boost::shared_ptr <BayesianInformationCriteria> BayesianInformationCriteriaPtr ;
};



#endif
