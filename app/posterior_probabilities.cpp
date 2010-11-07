#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "Symbol.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;


int main (int argc, char ** argv)
{

  srand(time(NULL));

  options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("model,m", value<string> (), "a decodable model")
    //    ("sequence,s", value<string> (), "sequence file")
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
	  if(vm.count("fasta") ) 
	    SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

    string model_name = vm["model"].as<string>();
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr model = creator.create(model_name);
    if(model == NULL)
    {
	    cerr << "Can't open model: " << model_name.c_str() << endl;
	    exit(-1);
    }

    AlphabetPtr alphabet = model->alphabet();
    AlphabetPtr states = model->decodable()->getStateNames();
    SequenceEntry entry(alphabet);
    while(!cin.eof()) 
    {
	    cin >> entry;
	    if(entry.getSequence().size() == 0) 
	      continue;
	    Matrix probabilities;
	    clock_t begin = clock();
	    model->decodable()->posteriorProbabilities(entry.getSequence(), probabilities);
	    clock_t end = clock();
	    std::cerr << "TIME: " << (double)(end - begin)/CLOCKS_PER_SEC << std::endl;
	    cout << entry.getName() << endl;
	    for (unsigned k = 0; k < probabilities.size1 (); k++)
	    {
        cout << states->getSymbol(k)->name() << "\t";
	      for (unsigned i = 0; i < probabilities.size2 (); i++)
	      {
          cout << probabilities(k, i) << "\t";
	      }
        cout << endl;
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
