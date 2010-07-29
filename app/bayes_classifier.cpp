#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "ProbabilisticModel.hpp"
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ConfigurationReader.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;


int main (int argc, char ** argv)
{

  srand(time(NULL));

  options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("configuration,c", value<string> (), "configuration file")
    ("fasta,F",  "use fasta format");


  try
    {
      variables_map vm;
      store(parse_command_line(argc, argv, desc), vm);
      notify(vm);
      if(vm.count("help"))
	{
	  cout << desc << "\n";
	  return 1;
	}
      string config_file = vm["configuration"].as<string>();
      if (vm.count("fasta")) 
	SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

      ConfigurationReader reader;
      std::ifstream input;
      std::string line;
      std::string conf;
      input.open(config_file.c_str());
      if(!input.is_open())
	{
	  std::cerr << "Cant open file "  << config_file << std::endl;
	  exit(-1);
	}
      while(!input.eof())
	{
	  getline(input,line,'\n');
	  line += "\n";
	  conf.append(line);
	}
      input.close();

      if(reader.load(conf))
	{
	  map<std::string,ProbabilisticModelPtr> models;
	  ProbabilisticModelParameterValuePtr classespar = (reader.parameters())->getMandatoryParameterValue("classes");
	  ProbabilisticModelParameterValuePtr modelprobspar = (reader.parameters())->getMandatoryParameterValue("model_probabilities");

	  if((modelprobspar == NULL) || (classespar == NULL))
	    exit(-1);

	  map<std::string,std::string> classes = classespar->getStringMap();
	  map<std::string,double> probabilities = modelprobspar->getDoubleMap();

	  map<std::string,std::string>::const_iterator it;
	  AlphabetPtr alphabet;
	  for(it = classes.begin(); it != classes.end(); it++)
	    {
	      ProbabilisticModelCreatorClient creator;
	      ProbabilisticModelPtr model = creator.create(it->second);
	      if(model == NULL)
		{
		  cerr << "Cannot open model: " << (it->first).c_str() << endl;
		  exit(-1);
		}
	      models[it->first] = model;
	      alphabet = model->alphabet();
	    }

	  SequenceEntry entry(alphabet);
	  cout << "sequence_name";
	  for(it = classes.begin(); it != classes.end(); it++)
	    {
	      cout << ";loglike(" << it->first<< ")";
	      cout << ";posterior(" << it->first << ")" ;
	    }
	  cout << ";class" ;
	  cout << endl;
	  while(!cin.eof()) {
	    cin >> entry;
	    if(entry.getSequence().size() == 0) 
	      continue;
	    map<std::string,ProbabilisticModelPtr>::const_iterator modelit;
	    stringstream oline;

	    double sum = -HUGE;
	    map<std::string, double> scores;
	    for(modelit = models.begin(); modelit != models.end(); modelit++)
	      {
		double score = (modelit->second)->evaluate(entry.getSequence(), 0, entry.getSequence().size()-1);
		scores[modelit->first] = score;
		sum = log_sum(sum, score+log(probabilities[modelit->first]));
	      }
	    double max = -HUGE;
	    std::string classified;
	    map<std::string, double>::const_iterator scoreit;
	    oline << entry.getName() << ";";
	    for(scoreit = scores.begin(); scoreit != scores.end(); scoreit ++)
	      {
		oline << scoreit->second << ";" << exp(scoreit->second + log(probabilities[scoreit->first]) - sum) << ";";
		if(max < (scoreit->second + log(probabilities[scoreit->first]) - sum) ) 
		  {
		    max =  (scoreit->second + log(probabilities[scoreit->first]) - sum) ;
		    classified = scoreit->first;
		  }
	      }
	    oline <<  classified;
	    cout << oline.str() << std::endl;
	  }
	}

    }
  catch (boost::program_options::invalid_command_line_syntax &e)
    {
      cerr << "error: " << e.what() << std::endl;
      cerr << desc << endl;
    }
  catch (boost::program_options::unknown_option &e)
    {
      cerr << "error: " << e.what() << std::endl;
      cerr << desc << endl;
    }
  catch (boost::bad_any_cast & e)
    {
      cerr << desc << endl;
    }
  return 0;
}
