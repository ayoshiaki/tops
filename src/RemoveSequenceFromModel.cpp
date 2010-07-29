#include "RemoveSequenceFromModel.hpp"
#include "SequenceFactory.hpp"
#include <map>
#include <vector>

namespace tops 
{
  ProbabilisticModelPtr RemoveSequenceFromModel::create( ProbabilisticModelParameters & parameters) const 
  {
    ProbabilisticModelParameterValuePtr seqlistpar = parameters.getMandatoryParameterValue("sequence_list");
    ProbabilisticModelParameterValuePtr alphabetpar = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr phasepar = parameters.getOptionalParameterValue("phase");

    if((seqlistpar == NULL) || (alphabetpar == NULL))
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromVector(alphabetpar->getStringVector());

    std::vector<std::string> seqs = seqlistpar->getStringVector();

    int phase = 0;
    if(phasepar != NULL) 
      phase = phasepar->getInt();
    

    ProbabilisticModelPtr result = _creator->create(parameters);
    for(int i = 0; i < (int)seqs.size() ; i ++ )
      {
	SequenceFactory factory(alphabet);
	boost::regex sep(" ");
	std::vector<std::string> seqstr;
	split_regex(seqs[i], seqstr,  sep);
	Sequence s = factory.createSequence(seqstr);
	result->removeSequenceFromModel(s, phase);
      }

    return result;
  }
}
