#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "util.hpp"
#include "ProbabilisticModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"
#include "DecodableModel.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "SparseMatrix.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;

void testFunc(ProbabilisticModelPtr model, Sequence seq)
{  
    SparseMatrixPtr a;

    model->decodable()->posteriorProbabilities(seq, a);
    //model->decodable()->backward(seq, a);
    /*    double sum = -HUGE;
      for(int i = 0; i < (int)a.size2(); i++){
      sum = -HUGE;
      for(int j = 0; j < (int)a.size1(); j++){
	sum = log_sum(sum,a(j,i));
	cout << "i = " << i << " j = " << j << " prob = " << a(j,i) << endl;
      }
      cout << sum << endl;
      }*/
}

int main (int argc, char ** argv)
{
    srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
      ("asequence,a", value<string> (), "sequence1 file")
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

	string model_file = vm["model"].as<string>();
	ProbabilisticModelCreatorClient creator;
	ProbabilisticModelPtr model = creator.create(model_file);
	if(model == NULL) 
	  {
	    exit(-1);
	  }

	if(vm.count("fasta") ) 
	  SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

	SequenceEntryList sample_set1; 
	AlphabetPtr alphabet = model->alphabet();
	readSequencesFromFile(sample_set1, alphabet, (vm["asequence"].as<string>()).c_str());
	Sequence seq;
	std::string name;
	seq = sample_set1[0]->getSequence();
	
	testFunc(model, seq);
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
