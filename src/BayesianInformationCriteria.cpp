#include "BayesianInformationCriteria.hpp"
#include <map>
#include <vector>

namespace tops 
{
  std::string BayesianInformationCriteria::help() const {
    std::stringstream out;
    out << "\nUSAGE: " << std::endl;
    out << "Mandatory parameters: " << std::endl;
    out << "\nbegin" << std::endl;
    out << "\tend" << std::endl;
    out << "\tstep" << std::endl;
    out << "Example: " << std::endl;
    out << "The configuration file below specify the BIC to select the WAM with the best order" << std::endl;
    out << "training_algorithm=\"WeightArrayModel\"" << std::endl;
    out << "training_set=\"dataset/sequences.txt\"" << std::endl;
    out << "alphabet=(\"A\", \"C\", \"G\", \"T\")" << std::endl;
    out << "length=31" << std::endl;
    out << "vicinity_length = 0" << std::endl;
    out << "pseudo_counts=0" << std::endl;
    out << "model_selection_criteria = \"BIC\"" << std::endl;
    out << "begin = (\"order\": 0)" << std::endl;
    out << "end = (\"order\": 3)" << std::endl;
    out << "step = (\"order\": 1)" << std::endl;
    return out.str();
  }

  ProbabilisticModelPtr BayesianInformationCriteria::create( ProbabilisticModelParameters & parameters) const 
  {
    ProbabilisticModelParameterValuePtr beginpar = parameters.getMandatoryParameterValue("begin");
    ProbabilisticModelParameterValuePtr endpar = parameters.getMandatoryParameterValue("end");
    ProbabilisticModelParameterValuePtr steppar = parameters.getMandatoryParameterValue("step");
    ProbabilisticModelParameterValuePtr trainpar = parameters.getMandatoryParameterValue("training_algorithm");
    
    if((beginpar == NULL) ||
       (endpar == NULL) ||
       (steppar == NULL)) 
      {
	std::cerr << help() << std::endl;
	exit(-1);
      }
    std::map<std::string, double> beginmap = beginpar->getDoubleMap();
    std::map<std::string, double> endmap = endpar->getDoubleMap();
    std::map<std::string, double> stepmap = steppar->getDoubleMap();
    std::vector <std::string> parnames;
    std::map <std::string, double>::const_iterator it;
    for(it = beginmap.begin(); it!=beginmap.end(); it++)
      parnames.push_back(it->first);

    // generates all possible combinations of parameters
    std::vector <std::string>::const_iterator p;
    std::vector <std::vector <double> > combinations;
    combinations.resize(1);
    for(p = parnames.begin(); p != parnames.end(); p++)
      {
	std::vector <std::vector <double> > new_combinations;
	for (double i = beginmap[*p] ; i <= endmap[*p]; i += stepmap[*p]) 
	  {
	    for(int j = 0; j < (int) combinations.size() ; j++)
	      {
		std::vector <double> comb;
		for(int k = 0; k < (int)combinations[j].size(); k++)
		  comb.push_back(combinations[j][k]);
		comb.push_back(i);
		new_combinations.push_back(comb);
	      }
	  }
	combinations = new_combinations;
      }						
    double bic = -HUGE;
    ProbabilisticModelPtr result;
    for(int i = 0; i < (int) combinations.size(); i++)
      {
	for(int j = 0; j < (int) combinations[i].size(); j++)
	  {
	    DoubleParameterValuePtr value = DoubleParameterValuePtr(new DoubleParameterValue(combinations[i][j]));
	    
	    parameters.set(parnames[j], value);
	  }
	double loglikelihood;
	int sample_size;
	ProbabilisticModelPtr m = _creator->create(parameters, loglikelihood, sample_size);
	double model_bic =  loglikelihood  - 0.5* m->size() * log((double) sample_size);
	if(bic < model_bic)
	  {
	    bic = model_bic;
	    result = m;
	  }

	int k = 0;
	if( k < (int) combinations[i].size()) 
	  {
	    std::cerr << combinations[i][k];
	    for(k = 1; k < (int) combinations[i].size(); k++)
	      std::cerr << "\t" << combinations[i][k];
	  }
	std::cerr << "\t" << model_bic  << std::endl;

      }
    std::cout << "# Model BIC: " << bic  << std::endl;
    return result;
  }
}
