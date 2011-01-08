/*
 *       TrainGHMMTransitions.hpp
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

#ifndef TRAIN_GHMM_TRANSITIONS_HPP
#define TRAIN_GHMM_TRANSITIONS_HPP

#include <boost/shared_ptr.hpp>
#include "ProbabilisticModelCreator.hpp"
namespace tops {

  class ProbabilisticModelParameters;

  //! This class is  a factory for the Multinomial Distribution
  class TrainGHMMTransitionsCreator : public ProbabilisticModelCreator 
  {
  public:
    TrainGHMMTransitionsCreator(){}
    virtual ~TrainGHMMTransitionsCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      return out.str();
    }


  };
  typedef boost::shared_ptr < TrainGHMMTransitionsCreator> TrainGHMMTransitionsCreatorPtr;
}

#endif
