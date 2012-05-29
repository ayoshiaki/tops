#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "ProbabilisticModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"
#include "MultipleAlignment.hpp"
#include "ProbabilisticModelCreatorClient.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;

int main (int argc, char ** argv)
{
    srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("outputDirectory,o", value<string> (), "name of directory where output files will be written in")
      ("alignmentmodel,a", value<string> (), "model")
      ("predictionmodels,p", value<string> (), "file with a list of prediction models (one for each sequence file)") 
      ("sequences,s", value<string> (), "file with a list of sequence files (one for each organism). Sequences that are to be aligned should be in the same position in the files")
      ("iterations,i", value<int> (), "number of consistency iterations")
      ("fasta,F",  "use fasta format")
      ("alternate",  "alternate between a round of alignment-prediction consistency, and a round of prediction-alignment consistency");

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
      
      string models_file = vm["predictionmodels"].as<std::string>();
      vector<ProbabilisticModelPtr> predmodels;
      ifstream fin;
      fin.open(models_file.c_str());
      while(!fin.eof()){
	ProbabilisticModelCreatorClient creator;
	string model_file;
	fin >> model_file;
	if(model_file.compare("") == 0){
	  continue;
	}
	cerr << model_file << endl;
	ProbabilisticModelPtr predmodel = creator.create(model_file);
	if(predmodel == NULL) 
	  {
	    cerr << "Could not create model from file " << model_file << endl;
	    exit(-1);
	  }
	predmodels.push_back(predmodel);
      }
      fin.close();
      
      string model_file = vm["alignmentmodel"].as<std::string>();
      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr almodel = creator.create(model_file);
      if(almodel == NULL) 
	{
	  exit(-1);
	}
      
      if(vm.count("fasta") ) 
	SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));	  
      
      int numit = 1;
      if(vm.count("iterations"))
	numit = vm["iterations"].as<int>();
      
      bool alternate = false;
      if(vm.count("alternate"))
	alternate = true;

      SparseMatrix::setppthresh(0.01);
   
      AlphabetPtr alphabet = almodel->alphabet();
      string outDir = vm["outputDirectory"].as<std::string>();

      string seqsFile = vm["sequences"].as<std::string>();
      vector<SequenceList> listOfSeqLists;
      vector< vector<string> > listOfSeqNamesLists;
      fin.open(seqsFile.c_str());
      while(!fin.eof()){
	string seqFileName;
	fin >> seqFileName;
	if(seqFileName.compare("") == 0 || seqFileName.compare("\n") == 0)
	  continue;
	cerr << seqFileName << endl;
	SequenceEntryList sample_set; 
	readSequencesFromFile(sample_set, alphabet, seqFileName.c_str());
	SequenceList seqs;
	std::vector<std::string> names;
	for(int i = 0; i < (int)sample_set.size(); i++){
	  seqs.push_back(sample_set[i]->getSequence());
	  names.push_back(sample_set[i]->getName());
	}
	listOfSeqLists.push_back(seqs);
	listOfSeqNamesLists.push_back(names);
      }
      fin.close();
      
      for(int i = 0; i < (int)listOfSeqLists[0].size(); i++){
	SequenceList seqs;
	std::vector<std::string> names;
	for(int j = 0; j < (int)listOfSeqLists.size(); j++){
	  seqs.push_back(listOfSeqLists[j][i]);
	  names.push_back(listOfSeqNamesLists[j][i]);
	}	
	MultipleAlignmentPtr ma = MultipleAlignmentPtr(new MultipleAlignment());
	ma->computePredictionsAndAlignment(almodel,predmodels,seqs,names,numit,alternate,outDir);
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
