/*
 *       viterbi_decoding.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonádio <ibonadio@ime.usp.br>
 *                      Vitor Onuchic <vitoronuchic@gmail.com>
 *                      Alan Mitchell Durham <aland@usp.br>
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

#include "ProbabilisticModel.hpp"
#include "DecodableModel.hpp"
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "version.hpp"
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
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";
            return 1;
        }
        if(vm.count("fasta") )
          SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

      string model_name = vm["model"].as<string>();
      //      string fasta = vm["sequence"].as<string>();
      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr model = creator.create(model_name);
      if(model == NULL)
        {
          cerr << "Can't open model: " << model_name.c_str() << endl;
          exit(-1);
        }

      AlphabetPtr alphabet = model->alphabet();
      //      std::ifstream input(fasta.c_str());
      //      if(!input.good()) {
      //        cerr << "Cant open file " << fasta << endl;
      //        exit(-1);
      //      }
      //      assert(input.good());
      SequenceEntry entry(alphabet);
      SequenceFormatManager::instance()->setFormat(SequenceFormatPtr(new SequenceFormat()));
      SequenceEntry output(model->decodable()->getStateNames());
      std::cin.rdbuf()->pubsetbuf(0, 0); // make it unbuffered
      while(!cin.eof()) {
        cin >> entry;
        if(entry.getSequence().size() == 0)
          continue;
        Sequence states;
        clock_t begin = clock();
        float ea = model->decodable()->MEAPred(entry.getSequence(), states);
        clock_t end = clock();
        std::cerr << "TIME: " << (double)(end - begin)/CLOCKS_PER_SEC << std::endl;
        stringstream new_name;
        new_name <<     entry.getName() <<  ": " << ea;
        output.setName(new_name.str());
        output.setSequence(states);
        std::cout.rdbuf()->pubsetbuf(0, 0); // make it unbuffered
        cout << output;
      }
      //      input.close();
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
