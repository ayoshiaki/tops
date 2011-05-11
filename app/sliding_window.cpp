/*
 *       sliding_window.cpp
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
    ("positive,m", value<string> (), "positive model")
    ("negative,n", value<string> (), "negative model")
    ("length,l", value<int> (), "length of the window" )
    ("phase,p", value<int> (), "phase of the model" )
    ("step,s", value<int> (), "step length" )
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
      if(vm.count("positive")<= 0) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";
        return -1;

      }
      if(vm.count("negative")<= 0) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";

        return -1;
      }
      if(vm.count("length")<= 0) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";
        cerr << "ERROR: Length of the window is a mandatory parameter" << std::endl;
        cerr << desc << "\n";
        return -1;
      }

      if (vm.count("fasta"))
        SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));

      ProbabilisticModelCreatorClient creator;
      ProbabilisticModelPtr positive = creator.create(vm["positive"].as<string>());
      ProbabilisticModelPtr negative = creator.create(vm["negative"].as<string>());
      int length = vm["length"].as<int>();
      int step = 1;
      int phase = 0;
      if(vm.count("phase")) {
        phase = vm["phase"].as<int>();
      }
      if(vm.count("step")){
        step = vm["step"].as<int>();
      }



      if((positive == NULL) || (negative == NULL)){
        std::cerr << "Cannot open models " << std::endl;
        exit(-1);
      }

      map<std::string,std::string>::const_iterator it;
      AlphabetPtr alphabet;
      alphabet = positive->alphabet();

      SequenceEntry entry(alphabet);
      while(!cin.eof()) {
        cin >> entry;
        if(entry.getSequence().size() == 0)
          continue;
        Sequence s = entry.getSequence();
        positive->initialize_prefix_sum_array(s);
        negative->initialize_prefix_sum_array(s);
        for(int i = 0; i < (int)s.size() - length + 1; i+=step)
          {
            double score = positive->prefix_sum_array_compute(i,i+length-1, phase);
            double neg_score = negative->prefix_sum_array_compute(i, i+length-1, phase);
            cout << i << "\t"
                 << entry.getName() << "\t"
                 << score << "\t"
                 << neg_score << "\t"
                 << score - neg_score << "\t"
                 << std::endl;
          }
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
