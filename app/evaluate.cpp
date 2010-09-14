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
    ("model,m", value<string> (), "model")
    ("phase,p", value<int> (), "phase of the model" )
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
      if(vm.count("model")<= 0) {
	cerr << desc << "\n";
	return -1;

      }
      if (vm.count("fasta")) 
	SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr m = creator.create(vm["model"].as<string>());      
      int phase = 0;
      if(vm.count("phase")) {
	phase = vm["phase"].as<int>();
      }



      if(m == NULL){
	std::cerr << "Cannot open model " << std::endl;
	exit(-1);
      }

      map<std::string,std::string>::const_iterator it;
      AlphabetPtr alphabet;
      alphabet = m->alphabet();

      SequenceEntry entry(alphabet);
      while(!cin.eof()) {
	cin >> entry;
	if(entry.getSequence().size() == 0) 
	  continue;
	Sequence s = entry.getSequence();
	double prob =  m->evaluate(s, 0, s.size() -1, phase);
	
	std::cout << entry.getName() << "\t"
		  << prob << std::endl;
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
