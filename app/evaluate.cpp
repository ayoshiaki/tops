/*
 *       evaluate.cpp
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
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ConfigurationReader.hpp"
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
    ("model,m", value<string> (), "model")
    ("phase,p", value<int> (), "phase of the model" )
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
          return 0;
        }
      if(vm.count("model")<= 0) {

            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
        cerr << desc << "\n";
        return 0;

      }
      if (vm.count("fasta"))
        SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr m = creator.create(vm["model"].as<string>());
      int phase = 0;
      if(vm.count("phase")) {
        phase = vm["phase"].as<int>();
      }



      if(m == NULL){
        std::cerr << "Cannot open model " << std::endl;
        exit(-1);
      }

      map<std::string,std::string>::const_iterator it;
      AlphabetPtr alphabet;
      alphabet = m->alphabet();

      SequenceEntry entry(alphabet);
      while(!cin.eof()) {
        cin >> entry;
        if(entry.getSequence().size() == 0)
          continue;
        Sequence s = entry.getSequence();
        double prob =  m->evaluate(s, 0, s.size() -1, phase);

#if 0
  m->initialize_prefix_sum_array(s,0);
  double prob2 = m->prefix_sum_array_compute(0, s.size()-1,0);
        std::cout << entry.getName() << "\t"
            << exp(prob) << " " << exp(prob2) <<std::endl;
#else
        std::cout << entry.getName() << "\t"
            << prob << std::endl;
#endif
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
