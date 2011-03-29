#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "ProbabilisticModel.hpp"
#include "PairHiddenMarkovModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;

void alignSeqs(ProbabilisticModelPtr model, SequenceList seqs1, SequenceList seqs2, vector<std::string> names1, vector<std::string> names2, ostream & output, ostream & states_output)
{  
  if(seqs1.size() != seqs2.size()){
    cerr << "Both files must to have the same amount of sequences." << endl;
    exit(-1);
  }
  for(int i = 0; i < (int)seqs1.size(); i++)
    {
      Sequence s1,s2,path;
      std::stringstream aux1,aux2,aux3;
      vector<Matrix> a;
      model->pairDecodable()->viterbi(seqs1[i], seqs2[i], path, s1, s2, a);
      aux1 << names1[i] << "_" << names2[i] << "_al1" ;
      aux2 << names1[i] << "_" << names2[i] << "_al2" ;
      aux3 << names1[i] << "_" << names2[i] << "_path" ;
      SequenceEntry out1(model->alphabet());
      SequenceEntry out2(model->alphabet());
      SequenceEntry out3(model->pairDecodable()->getStateNames());
      out1.setName(aux1.str());
      out2.setName(aux2.str());
      out3.setName(aux3.str());
      out1.setSequence(s1);
      out2.setSequence(s2);
      out3.setSequence(path);
      output << out1;
      output << out2 << endl;
      states_output << out3 << endl;
    }
}

int main (int argc, char ** argv)
{
    srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
      ("output,o", value<string>(), "file to store  the sequences")
      ("hidden_states,h", value<string>(), "file to store hidden state sequence")
      ("asequence,a", value<string> (), "sequence1 file")
      ("bsequence,b", value<string> (), "sequence2 file")
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

	SequenceEntryList sample_set1, sample_set2; 
	AlphabetPtr alphabet = model->alphabet();
	readSequencesFromFile(sample_set1, alphabet, (vm["asequence"].as<string>()).c_str());
	readSequencesFromFile(sample_set2, alphabet, (vm["bsequence"].as<string>()).c_str());
	SequenceList seqs1, seqs2;
	vector<std::string> names1, names2;
	for(int i = 0; i < (int)sample_set1.size(); i++){
	  seqs1.push_back(sample_set1[i]->getSequence());
	  names1.push_back(sample_set1[i]->getName());
	  seqs2.push_back(sample_set2[i]->getSequence());
	  names2.push_back(sample_set2[i]->getName());
	}
	
	if(vm.count("output")) {
	  string output = vm["output"].as<string>();
	  ofstream obs_output(output.c_str());
	  if(vm.count("hidden_states")) {
	    string hidden = vm["hidden_states"].as<string>();
	    ofstream hidden_output (hidden.c_str());
	    alignSeqs(model, seqs1, seqs2, names1, names2, obs_output, hidden_output);
	    hidden_output.close();	    
	  }
	  else {
	    alignSeqs(model, seqs1, seqs2, names1, names2, obs_output, std::cout);
	  }
	  obs_output.close();
	} 
	else { 
	  if(vm.count("hidden_states")) {
	    string hidden = vm["hidden_states"].as<string>();
	    ofstream hidden_output (hidden.c_str());
	    alignSeqs(model, seqs1, seqs2, names1, names2, std::cout, hidden_output);
	    hidden_output.close();
	  }
	  else 
	    alignSeqs(model, seqs1, seqs2, names1, names2, std::cout, std::cerr);
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
