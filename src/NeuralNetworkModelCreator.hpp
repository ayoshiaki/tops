/*
 *       NeuralNetworkModelCreator.hpp
 *
 *       Copyright 2011 Waldir Caro <waldirc@ime.usp.br>
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

#ifndef FINITE_NEURAL_NETWORK_CREATOR_HPP
#define FINITE_NEURAL_NETWORK_CREATOR_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include "NeuralNetworkModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the neural network model
  class DLLEXPORT NeuralNetworkModelCreator : public ProbabilisticModelCreator
  {
  public:
    NeuralNetworkModelCreator() {}
    virtual ~NeuralNetworkModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    //virtual NeuralNetworkModelPtr createNeuralNetworkModel(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tlayers = <a list of sequential layers>" << std::endl;
      out << "\tupstream_length = <upstream sequence length supported>" << std::endl;
      out << "\tdownstream_length = <downstream sequence length supported>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "\ttrained_model_file = <a file containing the trained neural network (layers could be empty)>" << std::endl;
      out << "Example: " << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr < NeuralNetworkModelCreator> NeuralNetworkModelCreatorPtr;
}

#endif
