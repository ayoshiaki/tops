/*
 *       simulateAlignment.cpp
 *
 *       Copyright 2011 Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Alan Mitchell Durham <aland@usp.br>
 *
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
#include "PairHiddenMarkovModel.hpp"
#include "SequenceEntry.hpp"
#include "SequenceFormat.hpp"

#include "ProbabilisticModelCreatorClient.hpp"
#include "version.hpp"
using namespace tops;
using namespace std;
using namespace boost::program_options;


void generate_sequence(std::string & model_file, int nseq, ostream & output, ostream & states_output)
{
  ProbabilisticModelCreatorClient creator;
  ProbabilisticModelPtr model = creator.create(model_file);
  if(model == NULL)
    {
      exit(-1);
    }
  for(int i = 0; i < nseq; i++)
    {
      Sequence s1,s2,path;
      std::stringstream aux1,aux2,aux3;
      model->pairDecodable()->generateSequence(s1, s2, path);
      aux1 << model_file << "_" << i << "_al1" ;
      aux2 << model_file << "_" << i << "_al2" ;
      aux3 << model_file << "_" << i << "_path" ;
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
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;

            cout << desc << "\n";
            return 1;
        }

        int nseq =  vm["numseq"].as<int>();
        string model = vm["model"].as<string>();

        if(vm.count("fasta") )
          SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

        if(vm.count("output")) {
          string output = vm["output"].as<string>();
          ofstream obs_output(output.c_str());
          if(vm.count("hidden_states")) {
            string hidden = vm["hidden_states"].as<string>();
            ofstream hidden_output (hidden.c_str());
            generate_sequence(model, nseq, obs_output, hidden_output);
            hidden_output.close();

          }
          else {
            generate_sequence(model, nseq, obs_output, std::cout);
          }
          obs_output.close();
        }
        else {
          if(vm.count("hidden_states")) {
            string hidden = vm["hidden_states"].as<string>();
            ofstream hidden_output (hidden.c_str());
            generate_sequence(model, nseq, std::cout, hidden_output);
            hidden_output.close();
          }
          else
            generate_sequence(model,  nseq, std::cout, std::cerr);
        }


    }
    catch (boost::program_options::invalid_command_line_syntax &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;
        cout << "error: " << e.what() << std::endl;
        cout << desc << endl;

    }
    catch (boost::program_options::unknown_option &e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;
        cout << "error: " << e.what() << std::endl;
        cout << desc << endl;
    }
    catch (boost::bad_any_cast & e)
    {
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;
        cout << desc << endl;
    }
    return 0;
}
