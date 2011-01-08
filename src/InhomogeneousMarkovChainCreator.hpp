/*
 *       InhomogeneousMarkovChainCreator.hpp
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

#ifndef IHM_CREATOR_HPP
#define IHM_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the variable length markov chain
  class InhomogeneousMarkovChainCreator : public ProbabilisticModelCreator 
  {
  public:
    InhomogeneousMarkovChainCreator() {}
    virtual ~InhomogeneousMarkovChainCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tposition_specific_distribution" << std::endl;
      out << "\tphased" << std::endl;
      out << "\talphabet" << std::endl;
      out << "Example: " << std::endl;
      out << "\tmodel_name=\"InhomogeneousMarkovChain\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\tprob1        = (\"0\" | \".\": 0.5;            # Pr(X_0 = 0)" << std::endl;
      out << "\t                \"1\" | \".\": 0.5;            # Pr(X_0 = 1)" << std::endl;
      out << "\t                \"0\" | \"0\": 0.1;            # Pr(X_n = 0 | X_{n-1} = 0)" << std::endl;
      out << "\t                \"1\" | \"0\": 0.9;            # Pr(X_n = 1 | X_{n-1} = 0)" << std::endl;
      out << "\t                \"0\" | \"1\": 0.5;            # Pr(X_n = 0 | X_{n-1} = 1)" << std::endl;
      out << "\t                \"1\" | \"1\": 0.5;            # Pr(X_n = 1 | X_{n-1} = 1)" << std::endl;
      out << "\t                \"0\" | \"1 0\": 0.7;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "\t                \"1\" | \"1 0\": 0.3;          # Pr(X_n = 1 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "\t                \"0\" | \"1 1\": 0.4;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;
      out << "\t                \"1\" | \"1 1\": 0.6)          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;
      out << "\tprob2        = (\"0\" | \".\": 0.5;            # Pr(X_0 = 0)" << std::endl;
      out << "\t                \"1\" | \".\": 0.5;            # Pr(X_0 = 1)" << std::endl;
      out << "\t                \"0\" | \"0\": 0.9;            # Pr(X_n = 0 | X_{n-1} = 0)" << std::endl;
      out << "\t                \"1\" | \"0\": 0.1;            # Pr(X_n = 1 | X_{n-1} = 0)" << std::endl;
      out << "\t                \"0\" | \"1\": 0.5;            # Pr(X_n = 0 | X_{n-1} = 1)" << std::endl;
      out << "\t                \"1\" | \"1\": 0.5;            # Pr(X_n = 1 | X_{n-1} = 1)" << std::endl;
      out << "\t                \"0\" | \"1 0\": 0.3;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "\t                \"1\" | \"1 0\": 0.7;          # Pr(X_n = 1 | X_{n-1} = 1, X_{n-2} = 0)" << std::endl;
      out << "\t                \"0\" | \"1 1\": 0.6;          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;
      out << " t                \"1\" | \"1 1\": 0.4)          # Pr(X_n = 0 | X_{n-1} = 1, X_{n-2} = 1)" << std::endl;
      out << "\tposition_specific_distribution = (\"prob1\", \"prob2\")" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr < InhomogeneousMarkovChainCreator> InhomogeneousMarkovChainCreatorPtr;
}

#endif
