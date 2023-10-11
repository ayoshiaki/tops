#include <caffe/caffe.hpp>
#include <boost/program_options.hpp>

#include <map>
#include <iostream>

#include "ProbabilisticModelCreatorClient.hpp"
#include "version.hpp"

using namespace std;
using namespace tops;
using namespace boost::program_options;

int main (int argc, char ** argv) {

    srand(time(NULL));


    try
    {
        string model_file = "model";
        ProbabilisticModelCreatorClient creator;
        ProbabilisticModelPtr model = creator.create(model_file);
        if(model == NULL)
          {
            exit(-1);
          }
/*
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
	  //alignSeqs(model, seqs1, seqs2, names1, names2, obs_output);
          obs_output.close();
        }
        else {
	  //alignSeqs(model, seqs1, seqs2, names1, names2, std::cout);
        }
*/
    }
    catch (boost::program_options::invalid_command_line_syntax &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;

        cerr << "error: " << e.what() << std::endl;

    }
    catch (boost::program_options::unknown_option &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;

        cerr << "error: " << e.what() << std::endl;

    }
    catch (boost::bad_any_cast & e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;

    }
    return 0;

}
