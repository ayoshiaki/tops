/*
 *       TrainHMMMaximumLikelihood.cpp
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
#include <sstream>
#include <iostream>
#include <fstream>
#include "Symbol.hpp"
#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainHMMMaximumLikelihood.hpp"
#include "TrainFixedLengthMarkovChain.hpp"
#include  "VariableLengthMarkovChain.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include <sstream>
namespace tops {
   std::string TrainHMMMaximumLikelihood::help() const {
     std::stringstream aux;
     aux << "training_algorithm=\"MaximumLikelihoodHMM\"" << std::endl;
     aux << "initial_specification=<file>" << std::endl;;
     aux << "training_set=<file>" << std::endl;
     return aux.str();
   }

  ProbabilisticModelPtr TrainHMMMaximumLikelihood::create( ProbabilisticModelParameters & parameters) const
  {
    ProbabilisticModelParameterValuePtr initialspecpar = parameters.getMandatoryParameterValue("initial_specification");
    ProbabilisticModelParameterValuePtr trainingsetpar = parameters.getMandatoryParameterValue("training_set");
    ProbabilisticModelParameterValuePtr pseudocontspar = parameters.getOptionalParameterValue("pseudo_counter");
    ProbabilisticModelCreatorClient client;
    ProbabilisticModelParameters initialspec = client.readConfigurationFromFile(initialspecpar->getString());
    ProbabilisticModelParameterValuePtr alphabetpar = initialspec.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr statespar = initialspec.getMandatoryParameterValue("state_names");

    ProbabilisticModelParameterValuePtr aprioriInitpar = initialspec.getOptionalParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr aprioriEmissionpar = initialspec.getOptionalParameterValue("emission_probabilities");
    ProbabilisticModelParameterValuePtr aprioriTransitionspar = initialspec.getOptionalParameterValue("transitions");
 
    double pseudocont = 0.0;
    if(pseudocontspar != NULL)
      pseudocont = pseudocontspar->getDouble();

    if((initialspecpar == NULL) || (trainingsetpar == NULL)){
      std::cerr << help() << std::endl;
      exit(-1);
    }


    AlphabetPtr alphabet = AlphabetPtr (new Alphabet());
    AlphabetPtr states = AlphabetPtr (new Alphabet());
    alphabet->initializeFromVector (alphabetpar->getStringVector());
    states->initializeFromVector(statespar->getStringVector());
    std::string filename(trainingsetpar->getString());
    std::ifstream input(filename.c_str());
    if(!input.good()) {
      std::cerr << "can not open file " << trainingsetpar->getString() << std::endl;
      exit(-1);
    }
    SequenceEntryList stateseqs;
    SequenceEntryList obserseqs;
    while(!input.eof()) {
	SequenceEntryPtr obsseq = SequenceEntryPtr(new SequenceEntry(alphabet));
	SequenceEntryPtr stateseq = SequenceEntryPtr(new SequenceEntry(states));
	input >> *obsseq;
	input >> *stateseq;
	if((obsseq->getSequence()).size() != (stateseq->getSequence()).size()) {
	  std::cerr << "ERROR: the observation sequence length is not equals to the state sequence length\n" << std::endl;
	  exit(-1);
	}
        stateseqs.push_back(stateseq);
        obserseqs.push_back(obsseq);
    }
    input.close();

    std::map<std::string,double> emisspar;
    std::map<std::string,double> transpar;
    std::map<std::string,double> initpar;
    std::map<std::string,double> aprioriEmiss;
    std::map<std::string,double> aprioriTrans; 
    std::map<std::string,double> aprioriInit;
    std::map<std::string,double>::const_iterator iter;

    if (aprioriEmissionpar != NULL) 
      aprioriEmiss = aprioriEmissionpar->getDoubleMap();
    if(aprioriTransitionspar != NULL)
      aprioriTrans = aprioriTransitionspar->getDoubleMap();
    if(aprioriInitpar != NULL)
      aprioriInit = aprioriInitpar->getDoubleMap();

   
    std::map<std::string,double>::const_iterator it;  

    Matrix pi (states->size(), 1);
    Matrix A (states->size(), states->size());
    Matrix E (states->size(), alphabet->size()); 
    
    for(int s = 0; s < states->size(); s++) {
      stringstream aux;
      aux << states->getSymbol(s)->name();
      pi(s, 0) = aprioriInit[aux.str()];
    }
    for(int from = 0; from < states->size(); from++) 
      for(int to = 0; to < states->size(); to++) {
	stringstream aux;
	aux << states->getSymbol(to)->name() << "|" << states->getSymbol(from)->name();
	A(from, to) = aprioriTrans[aux.str()];
      }

    for(int s = 0; s < states->size(); s++) 
      for(int o = 0; o < alphabet->size(); o++){
	stringstream aux1;
	aux1 << alphabet->getSymbol(o)->name() << "|" << states->getSymbol(s)->name();
	E(s,o) = aprioriEmiss[aux1.str()];
      }

    for(int i = 0; (i < obserseqs.size()) && (i < stateseqs.size()); i++) {
      Sequence obsseq = obserseqs[i]->getSequence();
      Sequence staseq = stateseqs[i]->getSequence();
      for(int j = 0; j < (j < obsseq.size()) && (j < staseq.size()); j++) {
	pi(staseq[j], 0) += 1.0;
	E(staseq[j], obsseq[j]) += 1.0; 
	if(j < staseq.size() - 1)
	  A(staseq[j], staseq[j+1]) += 1.0;
      }
    } 

    double sum = 0.0;

    for(int s = 0; s < states->size(); s++)
       sum += pi(s,0);
    for(int s = 0; s < states->size(); s++)
      pi(s, 0) /= sum;

    for(int from = 0; from < states->size(); from++){
      sum = 0;
      for(int to = 0; to < states->size(); to++)
	sum += A(from,to);
      for(int to = 0; to < states->size(); to++)
	A(from,to) /= sum;
    }
    sum = 0;
    for(int s = 0; s < states->size(); s++){
      for(int o = 0; o < alphabet->size(); o++)
	sum += E(s,o);
      for(int o = 0; o < alphabet->size(); o++)
	E(s,o) /= sum;
    }
  
    for(int s = 0; s < states->size(); s++)
      for(int o = 0; o < alphabet->size(); o++){
	stringstream aux;
	aux << alphabet->getSymbol(o)->name() << "|" << states->getSymbol(s)->name();
	emisspar[aux.str()] = E(s,o);
      }

    for(int s = 0; s < states->size(); s++) 
      initpar[states->getSymbol(s)->name()] = pi(s, 0);

    for(int from = 0; from < states->size(); from++)
      for(int to = 0; to < states->size(); to++){
	stringstream aux;
	aux << states->getSymbol(to)->name() << "|" << states->getSymbol(from)->name();
	transpar[aux.str()] = A(from, to);
      }

    ProbabilisticModelParameters hmm_parameters;
    hmm_parameters.add("model_name", ProbabilisticModelParameterValuePtr(new StringParameterValue("HiddenMarkovModel")));
    hmm_parameters.add("observation_symbols", alphabetpar);
    hmm_parameters.add("state_names", statespar);
    hmm_parameters.add("transitions", ProbabilisticModelParameterValuePtr(new DoubleMapParameterValue(transpar)));
    hmm_parameters.add("initial_probabilities",ProbabilisticModelParameterValuePtr(new DoubleMapParameterValue (initpar)));
    hmm_parameters.add("emission_probabilities",ProbabilisticModelParameterValuePtr(new DoubleMapParameterValue (emisspar)));
    ProbabilisticModelPtr m = client.create(hmm_parameters);
    return m;


  }
};



