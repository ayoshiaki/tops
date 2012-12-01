/*
 *       DiscreteIIDModelCreator.hpp
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

#ifndef FINITE_DISCRETE_DISTRIBUTION_CREATOR_HPP
#define FINITE_DISCRETE_DISTRIBUTION_CREATOR_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include "DiscreteIIDModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the finite discrete distribution
  class DLLEXPORT DiscreteIIDModelCreator : public ProbabilisticModelCreator
  {
  public:
    DiscreteIIDModelCreator() {}
    virtual ~DiscreteIIDModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual DiscreteIIDModelPtr createDiscreteIIDModel(ProbabilisticModelParameters & parameters) const ;

    virtual std::string help() const
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\tprobabilities = <a vector of doubles>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "\talphabet = <a vector of strings>" << std::endl;
      out << "Example: " << std::endl;
      out << "\tcreate_model=\"DiscreteIIDModel\"" << std::endl;
      out << "\toutput_model_file=\"./example.model\"" << std::endl;
      out << "\talphabet= (\"A\", \"C\", \"G\", \"T\")" << std::endl;
      out << "\tprobabilities= (0.25, 0.25, 0.25)" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr < DiscreteIIDModelCreator> DiscreteIIDModelCreatorPtr;
}

#endif
