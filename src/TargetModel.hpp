/*
 *       TargetModel.hpp
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

#ifndef TargetModel_H
#define TargetModel_H

#include "crossplatform.hpp"

#include "Sequence.hpp"
#include "Alphabet.hpp"

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "TargetModelCreator.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  //! This class is the Target Model
  class DLLEXPORT TargetModel : public ProbabilisticModel
  {
  public:

    TargetModel()  {
    };

    virtual ~TargetModel() {}
    virtual void initialize (const ProbabilisticModelParameters & p ) ;
    virtual ProbabilisticModelParameters parameters() const;
    virtual std::string model_name() const {
      return "TargetModel";
    }

    //! Calculates the sequence likelihood given this model
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;

    virtual ProbabilisticModelCreatorPtr getFactory () const{
          return TargetModelCreatorPtr(new TargetModelCreator());
    }

    virtual std::string str() const;


  };

  typedef boost::shared_ptr<TargetModel> TargetModelPtr;
}


#endif
