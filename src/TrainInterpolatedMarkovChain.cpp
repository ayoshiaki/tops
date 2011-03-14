/*
 *       TrainInterpolatedMarkovChain.cpp
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

#include "ProbabilisticModel.hpp"
#include "TrainInterpolatedMarkovChain.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "util.hpp"
namespace tops {

ProbabilisticModelPtr TrainInterpolatedMarkovChain::create(
                ProbabilisticModelParameters & parameters, double & loglikelihood,
                int & sample_size) const {
        ProbabilisticModelParameterValuePtr orderpar =
                        parameters.getMandatoryParameterValue("order");
        ProbabilisticModelParameterValuePtr trainpar =
                        parameters.getMandatoryParameterValue("training_set");
        ProbabilisticModelParameterValuePtr alphapar =
                        parameters.getMandatoryParameterValue("alphabet");
        ProbabilisticModelParameterValuePtr pseudocountspar = parameters.getOptionalParameterValue("pseudo_counts");
        ProbabilisticModelParameterValuePtr aprioripar = parameters.getOptionalParameterValue("apriori");

        double pseudocounts = 0;

        if(pseudocountspar != NULL)
          pseudocounts = pseudocountspar->getDouble();
        ProbabilisticModelPtr apriori;
        if(aprioripar != NULL)
            {
                ProbabilisticModelCreatorClient c;
                apriori = c.create(aprioripar->getString());
            }


        if ((trainpar == NULL) || (alphapar == NULL) || (orderpar == NULL)) {
          std::cerr << help() << std::endl;
          exit(-1);
        }
        AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
        alphabet ->initializeFromVector(alphapar->getStringVector());
        SequenceEntryList sample_set;
        readSequencesFromFile(sample_set, alphabet, trainpar->getString());
        ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
        tree->initializeCounter(sample_set, orderpar->getInt(), pseudocounts);
        tree->pruneTreeSmallSampleSize(400);

        if(apriori != NULL  && apriori->factorable() != NULL){
            tree->initializeCounter(sample_set, orderpar->getInt(), 0);
            tree->normalize(apriori, pseudocounts);
        } else {
            tree->initializeCounter(sample_set, orderpar->getInt(), pseudocounts);
            tree->normalize();
        }


        VariableLengthMarkovChainPtr m = VariableLengthMarkovChainPtr(
                                                                      new VariableLengthMarkovChain(tree));
        m->setAlphabet(alphabet);
        loglikelihood = 0.0;
        sample_size = 0;
        for (int i = 0; i < (int) sample_set.size(); i++) {
          loglikelihood
            += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
          sample_size += (sample_set[i]->getSequence()).size();
        }

        return m;

}
ProbabilisticModelPtr TrainInterpolatedMarkovChain::create(
                ProbabilisticModelParameters & parameters) const {
        double loglike;
        int samplesize;
        return create(parameters, loglike, samplesize);

}

}
;
