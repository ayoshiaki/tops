/*
 *       OverlappedProbabilisticModelCreator.hpp
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

#ifndef OVERLAP_MODEL_CREATOR_HPP
#define OVERLAP_MODEL_CREATOR_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelCreator.hpp"

namespace tops {
  class DLLEXPORT OverlappedProbabilisticModelCreator :public ProbabilisticModelCreator {
  public:
    //! Creates a probability model
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const ;
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const ;
    
    std::string help() const {
      std::stringstream out;
      out << "\nOVERLAPPED USAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\nleft" << std::endl;
      out << "\right" << std::endl;
      out << "\tsize" << std::endl;
      /*out << "Example: " << std::endl;
      out << "The configuration file below specify the AIC to select the WAM with the best order" << std::endl;
      out << "training_algorithm=\"WeightArrayModel\"" << std::endl;
      out << "training_set=\"dataset/sequences.txt\"" << std::endl;
      out << "alphabet=(\"A\", \"C\", \"G\", \"T\")" << std::endl;
      out << "length=31" << std::endl;
      out << "vicinity_length = 0" << std::endl;
      out << "pseudo_counts=0" << std::endl;
      out << "model_selection_criteria = \"AIC\"" << std::endl;
      out << "begin = (\"order\": 0)" << std::endl;
      out << "end = (\"order\": 3)" << std::endl;
      out << "step = (\"order\": 1)" << std::endl;*/
      return out.str();
    }
  };
  typedef boost::shared_ptr<OverlappedProbabilisticModelCreator> OverlappedProbabilisticModelCreatorPtr;
}

#endif
