/*
 *       TargetModelCreator.hpp
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

#ifndef TARGET_MODEL_CREATOR_HPP
#define TARGET_MODEL_CREATOR_HPP

#include "ProbabilisticModelCreator.hpp"
#include "ProbabilisticModel.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! This class is  a factory for the Target Model
  class TargetModelCreator : public ProbabilisticModelCreator 
  {
  public:
    TargetModelCreator() {}
    virtual ~TargetModelCreator(){};
    virtual ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters) const ;
    virtual std::string help() const 
    {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "model_name = \"TargetModel\"" << std::endl;
      out << "\talphabet = <a vector of strings>" << std::endl;
      out << "Optional parameters: " << std::endl;
      out << "Example: " << std::endl;
      out << "model_name = \"TargetModel\"" << std::endl;
      out << "\talphabet= (\"A\", \"C\", \"G\", \"T\")" << std::endl;
      return out.str();
    }

  };
  typedef boost::shared_ptr <TargetModelCreator> TargetModelCreatorPtr;
}

#endif
