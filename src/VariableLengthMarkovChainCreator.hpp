/*
 *       VariableLengthMarkovChainCreator.hpp
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

#ifndef VLMC_CREATOR_HPP
#define VLMC_CREATOR_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include "VariableLengthMarkovChain.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the variable length markov chain
  class DLLEXPORT VariableLengthMarkovChainCreator : public ProbabilisticModelCreator
  {
  public:
    VariableLengthMarkovChainCreator() {};
    virtual ~VariableLengthMarkovChainCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual VariableLengthMarkovChainPtr createVLMC(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tprobabilities" << std::endl;
      out << "\talphabet" << std::endl;
      out << "Example: " << std::endl;
      out << "\tmodel_name=\"VariableLengthMarkovChain\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "probabilities= (\"0\" | \".\": 0.5;            # Pr(X_0 = 0)" << std::endl;
      out << "                \"1\" | \".\": 0.5;            # Pr(X_0 = 1)" << std::endl;
      out << "                \"0\" | \"0\": 0.1;            # Pr(X_n = 0 | X_{n-1} = 0)" << std::endl;
      out << "                \"1\" | \"0\": 0.9;            # Pr(X_n = 1 | X_{n-1} = 0)" << std::endl;
      out << "                \"0\" | \"1\": 0.5;            # Pr(X_n = 0 | X_{n-1} = 1)" << std::endl;
      out << "                \"1\" | \"1\": 0.5;            # Pr(X_n = 1 | X_{n-1} = 1)" << std::endl;
      out << "                \"0\" | \"1 0\": 0.7;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "                \"1\" | \"1 0\": 0.3;          # Pr(X_n = 1 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "                \"0\" | \"1 1\": 0.4;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;
      out << "                \"1\" | \"1 1\": 0.6)          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;



      return out.str();
    }
    virtual std::string factory_name() const {
      return "VariableLengthMarkovChainCreator";
    }

  };
  typedef boost::shared_ptr < VariableLengthMarkovChainCreator> VariableLengthMarkovChainCreatorPtr;
}

#endif
