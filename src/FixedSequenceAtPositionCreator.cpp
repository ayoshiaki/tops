#include "FixedSequenceAtPositionCreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "FixedSequenceAtPosition.hpp"

namespace tops {
  ProbabilisticModelPtr FixedSequenceAtPositionCreator::create( ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr positionpar = parameters.getMandatoryParameterValue("position");
    ProbabilisticModelParameterValuePtr sequencepar = parameters.getMandatoryParameterValue("sequence");
    ProbabilisticModelParameterValuePtr probabilitypar = parameters.getMandatoryParameterValue("probability");
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
    boost::regex sep(" ");
    std::vector<std::string> seqstr;
    split_regex(sequencepar->getString(), seqstr,  sep);


    int position = positionpar->getInt();
    std::vector<double> probs;
    probs.push_back(probabilitypar->getDouble());
    probs.push_back(1.0 - probabilitypar->getDouble());
    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    std::string modelstr = modelpar->getString();

    ProbabilisticModelPtr m ;
    if((modelstr.size() > 0) && (modelstr[0] == '[') ){
      modelstr = modelstr.substr(1, modelstr.size() -2 );
      reader.load(modelstr);
      ProbabilisticModelParametersPtr par = reader.parameters();
      m = creator.create(*par);
    } else 
      {
	m = creator.create(modelstr) ;
	if(m == NULL) {
	  std::cerr << "Can not load model file " << modelstr<< "!" << std::endl;
	  exit(-1);
	}
      }
    FixedSequenceAtPositionPtr decorator = FixedSequenceAtPositionPtr(new FixedSequenceAtPosition(m));
    
    FiniteDiscreteDistributionPtr distr = FiniteDiscreteDistributionPtr(new FiniteDiscreteDistribution(probs));
    AlphabetPtr alpha = m->alphabet();
    SequenceFactory factory(alpha);
    Sequence sequence = factory.createSequence(seqstr);
    decorator->initialize(position, sequence, distr);
    decorator->setAlphabet(m->alphabet());
    decorator->subModelName(modelstr);
    return decorator;
  }

}
