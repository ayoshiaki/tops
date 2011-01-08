/*
 *       GeneralizedHiddenMarkovModelCreator.cpp
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

#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "MultinomialDistributionCreator.hpp"
#include "MultinomialDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "GeneralizedHiddenMarkovModel.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "ProbabilisticModelParameter.hpp"

#include <boost/algorithm/string.hpp>

namespace tops {
  ProbabilisticModelPtr GeneralizedHiddenMarkovModelCreator::create(
								    ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr state_names_par =
      parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr initial_probabilities_par =
      parameters.getMandatoryParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr transitions_par =
      parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr observation_symbols_par =
      parameters.getMandatoryParameterValue("observation_symbols");
    
    GeneralizedHiddenMarkovModelPtr m = GeneralizedHiddenMarkovModelPtr(
									new GeneralizedHiddenMarkovModel());
    
    if ((state_names_par == NULL) || (initial_probabilities_par == NULL)
	|| (transitions_par == NULL) || (observation_symbols_par == NULL)) {
      std::cerr << help() << std::endl;
      return m;
    }
    
    m->initialize(parameters);
    return m;

  }
  


}
