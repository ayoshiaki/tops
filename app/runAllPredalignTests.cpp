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
    ("inputFilesList,i", value<string> (), "file with the list of all files with sequences to be aligned")
    ("outputDirectory,o", value<string> (), "name of directory where output files will be written in")
    ("numberOfIterations,n", value<int> (), "number of consistency iterations")
    ("sparseMatrixCutoff,s", value<float> (), "cutoff for sparse matrices")
    ("fasta,F",  "use fasta format");
  
  try{
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    
    if(vm.count("help"))
      {
	cout << desc << "\n";
	return 1;
      }
    
    ProbabilisticModelCreatorClient creator;
    string model_file = vm["alignmentmodel"].as<std::string>();
    ProbabilisticModelPtr almodel = creator.create(model_file);
    if(almodel == NULL) 
      {
	exit(-1);
      }
    
    if(vm.count("fasta") ) 
      SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));
    
    string inputList = vm["inputFilesList"].as<std::string>();
    ifstream alFilesList;
    alFilesList.open(inputList.c_str());
    if(!alFilesList.is_open()){
      cerr << "Failed opening input file list" << endl;
      exit(-1);
    }
    while(alFilesList.good()){
      string alFileName;
      alFilesList >> alFileName;
      SequenceEntryList seqEntries; 
      AlphabetPtr alphabet = almodel->alphabet();
      readSequencesFromFile(seqEntries, alphabet, alFileName.c_str());
      SequenceList seqs;
      std::vector<std::string> names;
      for(int i = 0; i < (int)seqEntries.size(); i++){
	seqs.push_back(seqEntries[i]->getSequence());
	names.push_back(seqEntries[i]->getName());
      }
      
      int numit = 1;
      if(vm.count("numberOfIterations"))
	numit = vm["numberOfIterations"].as<int>();
      
      float scutoff = 0.01;
      if(vm.count("sparseMatrixCutoff"))
	scutoff = vm["sparseMatrixCutoff"].as<float>();
      SparseMatrix::setppthresh(scutoff);
      SparseMatrix::setcutthresh(scutoff);
      
      string outDir = vm["outputDirectory"].as<std::string>();
      
      MultipleAlignmentPtr ma = MultipleAlignmentPtr(new MultipleAlignment());
      ma->computeAllAlignments(almodel,seqs,names,numit,alFileName,outDir);
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
