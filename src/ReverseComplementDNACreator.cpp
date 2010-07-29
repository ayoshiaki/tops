#include "ReverseComplementDNACreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ReverseComplementDNA.hpp"

namespace tops {
  ProbabilisticModelPtr ReverseComplementDNACreator::create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
    ConfigurationReader reader;
    std::string modelstr = modelpar->getString();
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr m;
    std::map<std::string, ProbabilisticModelPtr>::const_iterator it = models.find(modelstr);
    bool frommodelname = false;
    if(it != models.end()) {
      m = it->second;
      frommodelname = true;
    }
    else{ 
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
    }
    ReverseComplementDNAPtr decorator = ReverseComplementDNAPtr(new ReverseComplementDNA(m));
    decorator->setAlphabet(m->alphabet());
    if(frommodelname)
      decorator->subModelName(modelstr);
    return decorator;
  }

  ProbabilisticModelPtr ReverseComplementDNACreator::create( ProbabilisticModelParameters & parameters) const {
    ProbabilisticModelParameterValuePtr modelpar = parameters.getMandatoryParameterValue("model");
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
    ReverseComplementDNAPtr decorator = ReverseComplementDNAPtr(new ReverseComplementDNA(m));

    decorator->setAlphabet(m->alphabet());

    return decorator;
  }


}
