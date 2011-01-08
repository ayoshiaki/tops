/*
 *       simulate.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *     
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU  General Public License as published by
 *       the Free Software Foundation; either version 3 of the License, or
 *       (at your option) any later version.
 *     
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *      
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the Free Software
 *       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *       MA 02110-1301, USA.
 */

#include <boost/program_options.hpp>
#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"

using namespace tops;
using namespace std;
using namespace boost::program_options;



void generate_sequence(std::string & model_file, int nseq, int length, ostream & output)
{
  
  ProbabilisticModelCreatorClient creator;
  ProbabilisticModelPtr model = creator.create(model_file);
  if(model == NULL) 
    {
      exit(-1);
    }
  for(int i = 0; i < nseq; i++)
    {
      Sequence s;
      std::stringstream aux;
      model->choose(s, length);
      aux << model_file << "_" << i ;
      SequenceEntry out(model->alphabet());
      out.setName(aux.str());
      out.setSequence(s);
      output << out;
    }
}

void generate_sequence(std::string & model_file, int nseq, int length, ostream & observation_output, ostream &states_output)
{
  ProbabilisticModelCreatorClient creator;
  ProbabilisticModelPtr model = creator.create(model_file);
  if(model == NULL) 
    {
      exit(-1);
    }
  
  for(int i = 0; i < nseq; i++)
    {
      Sequence o;
      Sequence y;
      o = model->choose(o, y, length);
      std::stringstream aux;
      aux << model_file << "_" << i ;
      SequenceEntry out(model->alphabet());
      out.setName(aux.str());
      out.setSequence(o);
      observation_output << out;

      SequenceEntry hiddenState(model->decodable()->getStateNames());
      hiddenState.setName(aux.str());
      hiddenState.setSequence(y);
      states_output << hiddenState;
    }
}



int main (int argc, char ** argv)
{
    srand(time(NULL));

    options_description desc("Allowed options");
    desc.add_options()
      ("help,?", "produce help message")
      ("model,m", value<string> (), "model")
      ("length,l", value<int> (), "length of the sequences")
      ("numseq,n", value<int> (), "number of sequences")
      ("output,o", value<string>(), "file to store  the sequences")
      ("hidden_states,h", value<string>(), "file to store hidden state sequence")
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

	int nseq =  vm["numseq"].as<int>();
	int length = vm["length"].as<int>();
	string model = vm["model"].as<string>();

	if(vm.count("fasta") ) 
	  SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));
	
	if(vm.count("output")) {
	  string output = vm["output"].as<string>();
	  ofstream obs_output(output.c_str());
	  if(vm.count("hidden_states")) {
	    string hidden = vm["hidden_states"].as<string>();
	    ofstream hidden_output (hidden.c_str());
	    generate_sequence(model, nseq, length, obs_output, hidden_output);
	    hidden_output.close();
	    
	  }else {
	    generate_sequence(model,  nseq,  length, obs_output);
	  }
	  obs_output.close();
	} else { 
	  if(vm.count("hidden_states")) {
	    string hidden = vm["hidden_states"].as<string>();
	    ofstream hidden_output (hidden.c_str());
	    generate_sequence(model, nseq, length, std::cout, hidden_output);
	    hidden_output.close();
	  }else 
	    generate_sequence(model,  nseq,  length, std::cout);
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
