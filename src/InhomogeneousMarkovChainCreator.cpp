/*
 *       InhomogeneousMarkovChainCreator.cpp
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

#include "InhomogeneousMarkovChainCreator.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "Alphabet.hpp"
#include <boost/algorithm/string.hpp>
namespace tops {

  ProbabilisticModelPtr InhomogeneousMarkovChainCreator::create(ProbabilisticModelParameters & parameters) const {
    InhomogeneousMarkovChainPtr model = InhomogeneousMarkovChainPtr(new InhomogeneousMarkovChain());
    ProbabilisticModelParameterValuePtr symbols = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr position_specific_model_names = parameters.getMandatoryParameterValue("position_specific_distribution");
    ProbabilisticModelParameterValuePtr p = parameters.getMandatoryParameterValue("phased");
    if((symbols == NULL) || (position_specific_model_names == NULL) || (p == NULL))
      {
	std::cerr << help() << std::endl;
      }
    model->initialize(parameters);
    return model;
  }


}
