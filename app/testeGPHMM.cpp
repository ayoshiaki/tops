#include <boost/program_options.hpp>
#include <boost/timer.hpp>
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
#include "util.hpp"
#include "SparseMatrix.hpp"

#include "ProbabilisticModelCreatorClient.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;

void testeVector(){
  float t1,t2;
  boost::timer t;
  double sf = 0;
  t.restart();
  vector<Matrix> m(5);
  for(int k = 0; k < 5; k++)
    m[k].resize(1000,1000);
  
  for(int k = 0; k < 5; k++){
    for(int i = 0; i < 1000; i++){
      for(int j = 0; j < 1000; j++){
	m[k](i,j) = 1.0;
      }
    }
  }
  for(int k = 0; k < 5; k++){
    for(int i = 0; i < 1000; i++){
      for(int j = 0; j < 1000; j++){
	sf += m[k](i,j);
      }
    }
  }
  t1 = t.elapsed();
  cout << sf << endl;

  sf = 0;
  t.restart();
  vector<double> v(5000000);
  for(int i = 0; i < 5000000; i++){
    v[i] = 1;
  }
  for(int i = 0; i < 5000000; i++){
    sf += v[i];
  }
  t2 = t.elapsed();
  cout << sf << endl;

  cout << "t1 = " << t1 << " t2 = " << t2 << endl;
}

/*void testFunc(ProbabilisticModelPtr model, SequenceList seqs1, SequenceList seqs2)
{  
  if(seqs1.size() != seqs2.size()){
    cerr << "Both files must to have the same amount of sequences." << endl;
    exit(-1);
  }
  for(int i = 0; i < (int)seqs1.size(); i++){
    vector<CMatrix> a,b;
    model->pairDecodable()->posteriorProbabilities(seqs1[i],seqs2[i],a);*/
    /*for(int m = 0; m <= (int)seqs1[i].size(); m++){
      for(int n = 0; n <= (int)seqs2[i].size(); n++){
	double sum = -HUGE;
	for(int o = 0; o < (int)a.size(); o++){
	  sum = log_sum(sum, a[o](m,n));
	}
	cout << sum << endl;
      }
      }*/
      
    //double score = model->pairDecodable()->backward(seqs1[i], seqs2[i], a);
    /*cout << "backward" << endl;
    for(int o = 0; o < (int)a.size(); o++){
      for(int m = 0; m <= (int)seqs1[i].size(); m++){
	for(int n = 0; n <= (int)seqs2[i].size(); n++){
	  if(n == (int)seqs2[i].size()){
	    cout << a[o](m,n) << endl;
	  }
	  else{
	    cout << a[o](m,n) << " ";
	  }
	}
      }
      cout << endl;
    }
    cout << "forward" << endl;
    double score2 = model->pairDecodable()->forward(seqs1[i], seqs2[i], a);
    for(int o = 0; o < (int)a.size(); o++){
      for(int m = 0; m <= (int)seqs1[i].size(); m++){
	for(int n = 0; n <= (int)seqs2[i].size(); n++){
	  if(n == (int)seqs2[i].size()){
	    cout << a[o](m,n) << endl;
	  }
	  else{
	    cout << a[o](m,n) << " ";
	  }
	}
      }
      cout << endl;
      }
    cout << "backward=" << score << " forward=" << score2 << endl;
  }
  }*/

    /*void testeSparseMatrix(){
  SparseMatrixPtr M = SparseMatrixPtr(new SparseMatrix(3,5));
  SparseMatrixPtr N = SparseMatrixPtr(new SparseMatrix(5,3));
  SparseMatrixPtr R = SparseMatrixPtr(new SparseMatrix(3,3));
  M->add(0,0,0.4);
  M->add(1,2,0.5);
  M->add(0,4,0.8);
  M->add(1,1,0.3);
  M->add(1,3,0.1);
  M->add(2,4,0.9);

  N->add(0,0,0.4);
  N->add(0,1,0.3);
  N->add(2,0,0.5);
  N->add(0,1,1.0);
  N->add(1,1,0.3);
  N->add(3,1,1.0);

  
  R->sum_prod(M,N,1.0);
  M->printFullMatrix();
  cout << endl;
  N->printFullMatrix();
  cout << endl;
  R->printFullMatrix();
  }*/

int main (int argc, char ** argv)
{
  testeVector();
  /*srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
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
	
	testFunc(model, seqs1, seqs2);
  
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
    return 0;*/
}
