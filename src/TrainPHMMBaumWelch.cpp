#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainPHMMBaumWelch.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "PairHiddenMarkovModel.hpp"
namespace tops {

  ProbabilisticModelPtr TrainPHMMBaumWelch::create( ProbabilisticModelParameters & parameters) const 
  {
    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr trainpar1 = parameters.getMandatoryParameterValue("training_set1");
    ProbabilisticModelParameterValuePtr trainpar2 = parameters.getMandatoryParameterValue("training_set2");
    ProbabilisticModelParameterValuePtr thrpar = parameters.getOptionalParameterValue("threshold");
    ProbabilisticModelParameterValuePtr maxiterpar = parameters.getOptionalParameterValue("maxiter");
    double threshold = 1e-5;
    if(thrpar != NULL)
      threshold = thrpar->getDouble();
    int maxiter = 500;
    if(maxiterpar != NULL)
      maxiter = maxiterpar->getInt();
      
    ProbabilisticModelCreatorClient creator;
    std::string name = initmodelpar->getString();
    ProbabilisticModelPtr m = creator.create(name);
    SequenceEntryList sample_set1, sample_set2; 
    AlphabetPtr alphabet = m->alphabet();
    readSequencesFromFile(sample_set1, alphabet, trainpar1->getString());
    readSequencesFromFile(sample_set2, alphabet, trainpar2->getString());
    SequenceList seqs1, seqs2;
    for(int i = 0; i < (int)sample_set1.size(); i++)
      seqs1.push_back(sample_set1[i]->getSequence());
    for(int i = 0; i < (int)sample_set2.size(); i++)
      seqs2.push_back(sample_set2[i]->getSequence());
    m->pairDecodable()->trainBaumWelch(seqs1, seqs2, maxiter, threshold);
    return m;
  }
};
