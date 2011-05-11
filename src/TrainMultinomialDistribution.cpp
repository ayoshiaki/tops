/*
 *       TrainMultinomialDistribution.cpp
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

#include "ProbabilisticModel.hpp"
#include "TrainMultinomialDistribution.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "util.hpp"
namespace tops {

  ProbabilisticModelPtr TrainMultinomialDistribution::create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & sample_set, double & loglikelihood, int & sample_size) const {

        ProbabilisticModelParameterValuePtr alphapar =
                        parameters.getMandatoryParameterValue("alphabet");


        if (alphapar == NULL) {
          std::cerr << help() << std::endl;
          exit(-1);
        }
        AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
        alphabet ->initializeFromVector(alphapar->getStringVector());
        SequenceEntryList samples;
        SequenceFactory factory(alphabet);
        for(int i = 0; i < (int)sample_set.size();i++){
          SequenceEntryPtr  inseq = SequenceEntryPtr(new SequenceEntry(alphabet));
          inseq->setSequence(factory.createSequenceRemovedSpaces(sample_set[i]));
          samples.push_back(inseq);
        }

        ProbabilisticModelPtr m = train (samples, alphabet);
        loglikelihood = 0.0;
        sample_size = 0;
        for (int i = 0; i < (int) samples.size(); i++) {
          loglikelihood
            += m->evaluate((samples[i]->getSequence()), 0, (samples[i]->getSequence()).size() - 1);
          sample_size += (samples[i]->getSequence()).size();
        }
        return m;
  }

ProbabilisticModelPtr TrainMultinomialDistribution::train(const SequenceEntryList & sample_set, AlphabetPtr alphabet) const{
        ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
        std::map <std::string, double> w;
        tree->initializeCounter(sample_set, 0, w);
        tree->normalize();
        MultinomialDistributionPtr m = tree->getRoot()->getDistribution();
        m->setAlphabet(alphabet);
        return m;
  }

  ProbabilisticModelPtr TrainMultinomialDistribution::create(
                ProbabilisticModelParameters & parameters, double & loglikelihood,
                int & sample_size) const {
        ProbabilisticModelParameterValuePtr trainpar =
                        parameters.getMandatoryParameterValue("training_set");
        ProbabilisticModelParameterValuePtr alphapar =
                        parameters.getMandatoryParameterValue("alphabet");

        if ((trainpar == NULL) || (alphapar == NULL) ) {
          std::cerr << help() << std::endl;
          exit(-1);
        }
        AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
        alphabet ->initializeFromVector(alphapar->getStringVector());
        SequenceEntryList sample_set;
        readSequencesFromFile(sample_set, alphabet, trainpar->getString());
        ProbabilisticModelPtr m =  train(sample_set, alphabet);
        loglikelihood = 0.0;
        sample_size = 0;
        for (int i = 0; i < (int) sample_set.size(); i++) {
          loglikelihood
            += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
          sample_size += (sample_set[i]->getSequence()).size();
        }
        return m;

}
  ProbabilisticModelPtr TrainMultinomialDistribution::create(
                                                                ProbabilisticModelParameters & parameters) const {
    double loglike;
    int samplesize;
    return create(parameters, loglike, samplesize);

  }

   std::string TrainMultinomialDistribution::help() const   {
      std::stringstream out;
      out << "\nUSAGE: " << std::endl;
      out << "Mandatory parameters: " << std::endl;
      out << "\ttraining_set" << std::endl;
      out << "\talphabet" << std::endl;
      out << "Example: " << std::endl;
      out << "\ttraining_algorithm=\"MultinomialDistribution\"" << std::endl;
      out << "\talphabet=(\"0\", \"1\")" << std::endl;
      out << "\ttraining_set= \"input.seq" << std::endl;
      return out.str();
  }
}

