/*
 *       evaluate.cpp
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

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "SequenceEntry.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "ConfigurationReader.hpp"

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
      if(vm.count("model")<= 0) {
        cerr << desc << "\n";
        return -1;

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
        std::vector <int> invalidPositions = entry.invalidPositions();
        for(int i = 0; i < (int)invalidPositions.size(); i++)
            {
                double max = -HUGE;
                int j = invalidPositions[i];
                int g = 0;
                for(int k = 0; k < (int)alphabet->size(); k++) {
                    s[j] = k;
                    double v =  m->factorable()->evaluatePosition(s, j);
                    if (max < v){
                        max = v;
                        g = k;
                    }
                }
                s[j] = g;
            }
        entry.setSequence(s);
        cout << entry;
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
