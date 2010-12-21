#include "ConfigurationReader.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "PhasedRunLengthDistribution.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
namespace tops {

  ProbabilisticModelParameters PhasedRunLengthDistribution::parameters() const
  {
    ProbabilisticModelParameters p;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name())));
    p.add("input_phase", IntParameterValuePtr(new IntParameterValue(_iphase)));
    p.add("output_phase", IntParameterValuePtr(new IntParameterValue(_ophase)));
    p.add("number_of_phases", IntParameterValuePtr(new IntParameterValue(_nphases)));
    p.add("delta", DoubleParameterValuePtr(new DoubleParameterValue(_delta)));
    std::string modelname = ProbabilisticModelDecorator::subModelName();
    if(modelname.length() > 0) 
      p.add("model", StringParameterValuePtr(new StringParameterValue(modelname)));
    else
      p.add("model", StringParameterValuePtr(new StringParameterValue(subModel()->str())));
    
    return p;
  }
  
  void PhasedRunLengthDistribution::initialize(const ProbabilisticModelParameters & p)
  {
    ProbabilisticModelParameterValuePtr iphasepar = p.getMandatoryParameterValue("input_phase");
    ProbabilisticModelParameterValuePtr ophasepar = p.getMandatoryParameterValue("output_phase");
    ProbabilisticModelParameterValuePtr nphasepar = p.getMandatoryParameterValue("number_of_phases");
    ProbabilisticModelParameterValuePtr  deltapar = p.getMandatoryParameterValue("delta");
    ProbabilisticModelParameterValuePtr  modelpar = p.getMandatoryParameterValue("model");

    int iphase = iphasepar -> getInt();
    int ophase = ophasepar -> getInt();
    int nphase = nphasepar -> getInt();
    int delta = deltapar -> getInt();
    std::string modelstr = modelpar->getString();

    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    ProbabilisticModelPtr m ;
    if((modelstr.size() > 0) && (modelstr[0] == '[') ){
      modelstr = modelstr.substr(1, modelstr.size() -2 );
      reader.load(modelstr);
      ProbabilisticModelParametersPtr par = reader.parameters();
      m = creator.create(*par);
    } else  {
	m = creator.create(modelstr) ;
	if(m == NULL) {
	  std::cerr << "Can not load model file " << modelstr<< "!" << std::endl;
	  exit(-1);
	}
      }
    AlphabetPtr alpha = m->alphabet();
    initialize(delta, iphase, ophase, nphase);
    setAlphabet(m->alphabet());
    setSubModel(m);
  }


  void PhasedRunLengthDistribution::initialize(int delta, int input_phase, int output_phase, int nphase)
  {
    _delta = delta;
    _iphase = input_phase;
    _ophase = output_phase;
    _nphases = nphase;
    double sum = -HUGE;
    for(int i = 0; i < subModel()->size(); i++) {
      int d = i + _delta;
      if(mod((d + _iphase-1), _nphases) == _ophase) 
	sum = log_sum(sum, subModel()->log_probability_of(i));
    }
    _normfactor = sum;
  }
  double PhasedRunLengthDistribution::choose() const
  {
    int L = (int)(ProbabilisticModelDecorator::choose());
    while(mod((L + _iphase-1), _nphases ) != _ophase) {L++;}

    int d = L - _delta;
    if (d < 1) {
      L = _delta+1;
      while(mod((L + _iphase-1), _nphases ) != _ophase) {L++;}
      d = L - _delta;
    }

    return d;
  }
  
  double  PhasedRunLengthDistribution::log_probability_of(int s) const
  {
    int d = s + _delta;
    if(mod((d + _iphase-1), _nphases) != _ophase) 
      return -HUGE;
    double result = subModel()->log_probability_of(d);
    return result-_normfactor;
  }
  
  Sequence & PhasedRunLengthDistribution::choose(Sequence & h, int size) const {
    for(int i = 0; i < size; i++)
      h.push_back(choose());
    return h;
  }

std::string PhasedRunLengthDistribution::str() const{
  std::stringstream out;
  out << "model_name =\"" << model_name() << "\"" << std::endl;
  out << "delta = " << _delta << std::endl;
  out << "input_phase = " << _iphase << std::endl;
  out << "output_phase = " << _ophase << std::endl;
  out << "number_of_phases = " << _nphases << std::endl;
  std::string modelname = ProbabilisticModelDecorator::subModelName();
  if(modelname.length() > 0) 
    out << "model = " << modelname << std::endl ;
  else
    out << "model = [" << subModel()->str() << "]" << std::endl ;
  return out.str();
}
std::string PhasedRunLengthDistribution::model_name () const{
  return "PhasedRunLengthDistribution";
}
  double PhasedRunLengthDistribution::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const{
    double result = 0.0;
    for(int i = begin; (i < (int)s.size()) && (i <= (int)end); i++)
      result += log_probability_of(s[i]);
    return result;
  }

}

