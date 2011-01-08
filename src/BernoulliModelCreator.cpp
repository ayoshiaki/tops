/*
 *       BernoulliModelCreator.cpp
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

#include "BernoulliModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "MultinomialDistribution.hpp"
#include "Alphabet.hpp"

namespace tops {
  ProbabilisticModelPtr BernoulliModelCreator::create(ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue("probability");
    if(probs == NULL)
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    DoubleVector distr;
    distr.push_back(probs->getDouble());
    distr.push_back(1.0 - probs->getDouble());
    ProbabilisticModelPtr model = MultinomialDistributionPtr(new MultinomialDistribution(distr));
    return model;
  }
}
