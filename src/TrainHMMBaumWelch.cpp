#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainHMMBaumWelch.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
namespace tops {

  ProbabilisticModelPtr TrainHMMBaumWelch::create( ProbabilisticModelParameters & parameters) const 
  {
    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
    ProbabilisticModelParameterValuePtr trainpar = parameters.getMandatoryParameterValue("training_set");
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
    SequenceEntryList sample_set; 
    AlphabetPtr alphabet = m->alphabet();
    readSequencesFromFile(sample_set, alphabet, trainpar->getString());
    SequenceList seqs;
    for(int i = 0; i < (int)sample_set.size(); i++)
      seqs.push_back(sample_set[i]->getSequence());
    m->trainBaumWelch(seqs, maxiter, threshold);
    return m;
  }
};



