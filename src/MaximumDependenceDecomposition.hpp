/*
 *       InhomogeneousMarkovChain.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
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

#ifndef MAXIMUM_DEPENDENCE_DECOMPOSITION_HPP
#define MAXIMUM_DEPENDENCE_DECOMPOSITION_HPP

#include "crossplatform.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "ContextTree.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  
  
  //! Maximum Dependence Decomposition: not implemented yet 
  class DLLEXPORT MaximumDependenceDecomposition : public ProbabilisticModel
  {
  public:
    MaximumDependenceDecomposition(){
    }
    virtual std::string model_name() const {
      return "MaximumDependenceDecomposition";
    }

    //! Calculates the sequence likelihood given this model
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual void initialize(const ProbabilisticModelParameters & p)  ;
    virtual ProbabilisticModelParameters parameters() const;
    virtual Sequence & choose(Sequence & h, int size) const;
    virtual std::string str() const;
  private:
    int _max_size;
    std::map <std::string,ProbabilisticModelPtr> _submodels;
    std::map <std::string,int> _consensus;
    std::map <std::string,int> _positions;
    ProbabilisticModelParameters _parameters;
  };

  typedef boost::shared_ptr <MaximumDependenceDecomposition> MaximumDependenceDecompositionPtr;

}


#endif
