/*
 *       RemoveSequenceFromModel.hpp
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

#ifndef REMOVE_SEQUENCE_FROM_MODEL_HPP
#define REMOVE_SEQUENCE_FROM_MODEL_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"

namespace tops
{
  class DLLEXPORT RemoveSequenceFromModel : public ProbabilisticModelCreator {
  private:
    ProbabilisticModelCreatorPtr _creator;
  public:
    RemoveSequenceFromModel (){}

    RemoveSequenceFromModel (ProbabilisticModelCreatorPtr creator) : _creator(creator) {}
    virtual ~RemoveSequenceFromModel () {};
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const {
      std::string s;
      return s;
    }
    virtual std::string factory_name() const {
      return "BayesianInformationCriteria";
    }
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      _creator = creator;
    }

  };
  typedef boost::shared_ptr <RemoveSequenceFromModel> RemoveSequenceFromModelPtr ;
};



#endif
