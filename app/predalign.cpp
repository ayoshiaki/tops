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
      ("alignmentmodel,a", value<string> (), "model")
      ("predictionmodel,p", value<string> (), "model") 
      ("consistencyscheme,c", value<int> (), "consistency transformation to be used\n1 = gapped consistency and alignment construction\n2 = gapped consistency only\n3 = picxaa consistency\n4 = classic consistency")  
      ("sequences,s", value<string> (), "sequence file")
      ("iterations,i", value<int> (), "number of consistency iterations")
      ("getprobs,g", value<string> (), "train, calculate posterior probabilities and store them in given file")
      ("readfromfile,r", value<string> (), "read posterior probabilities from file")
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
      
      string model_file = vm["predictionmodel"].as<std::string>();
      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr predmodel = creator.create(model_file);
      if(predmodel == NULL) 
	{
	  exit(-1);
	}
      
      model_file = vm["alignmentmodel"].as<std::string>();
      ProbabilisticModelPtr almodel = creator.create(model_file);
      if(almodel == NULL) 
	{
	  exit(-1);
	}
      
      if(vm.count("fasta") ) 
	SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));	  

      if(vm.count("getprobs")){
	string outFile = vm["getprobs"].as<std::string>();
	SequenceEntryList sample_set; 
	AlphabetPtr alphabet = almodel->alphabet();
	readSequencesFromFile(sample_set, alphabet, (vm["sequences"].as<std::string>()).c_str());
	SequenceList seqs;
	std::vector<std::string> names;
	for(int i = 0; i < (int)sample_set.size(); i++){
	  seqs.push_back(sample_set[i]->getSequence());
	  names.push_back(sample_set[i]->getName());
	}
	SparseMatrix::setppthresh(0.01);
	MultipleAlignmentPtr ma = MultipleAlignmentPtr(new MultipleAlignment());
      	//ma->trainAndComputePPs(model_file, seqs, names, 0, outFile);
	return 0;
      }

      if(vm.count("readfromfile")){
	string inFile = vm["readfromfile"].as<std::string>();
	int consScheme = 0;
	consScheme = vm["consistencyscheme"].as<int>();
	int numit = 1;
	numit = vm["iterations"].as<int>();
	SequenceEntryList sample_set; 
	AlphabetPtr alphabet = almodel->alphabet();
	readSequencesFromFile(sample_set, alphabet, (vm["sequences"].as<std::string>()).c_str());
	SequenceList seqs;
	std::vector<std::string> names;
	for(int i = 0; i < (int)sample_set.size(); i++){
	  seqs.push_back(sample_set[i]->getSequence());
	  names.push_back(sample_set[i]->getName());
	}
	
	map<string,ProbabilisticModelPtr> predmodels;
	for(int i = 0; i < (int)names.size(); i++)
	  predmodels[names[i]] = predmodel;
	SparseMatrix::setppthresh(0.01);
	MultipleAlignmentPtr ma = MultipleAlignmentPtr(new MultipleAlignment());
	//ma->initializeFromFile(almodel,seqs,names,numit,consScheme,inFile);
	return 0;
      }	
      
      int consScheme = 0;
      consScheme = vm["consistencyscheme"].as<int>();
      int numit = 1;
      numit = vm["iterations"].as<int>();
     
      SequenceEntryList sample_set; 
      AlphabetPtr alphabet = almodel->alphabet();
      readSequencesFromFile(sample_set, alphabet, (vm["sequences"].as<std::string>()).c_str());
      SequenceList seqs;
      std::vector<std::string> names;
      for(int i = 0; i < (int)sample_set.size(); i++){
	seqs.push_back(sample_set[i]->getSequence());
	names.push_back(sample_set[i]->getName());
      }
      
      map<string,ProbabilisticModelPtr> predmodels;
      for(int i = 0; i < (int)names.size(); i++)
	predmodels[names[i]] = predmodel;

      SparseMatrix::setppthresh(0.01);

      MultipleAlignmentPtr ma = MultipleAlignmentPtr(new MultipleAlignment());
      
      ma->computeOneAlignment(almodel,predmodels,seqs,names,numit,consScheme);	
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
