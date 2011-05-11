/*
 *       posterior_probabilities.cpp
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
#include "Symbol.hpp"
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
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr model = creator.create(model_name);
    if(model == NULL)
    {
            cerr << "Can't open model: " << model_name.c_str() << endl;
            exit(-1);
    }

    AlphabetPtr alphabet = model->alphabet();
    AlphabetPtr states = model->decodable()->getStateNames();
    SequenceEntry entry(alphabet);
    while(!cin.eof())
    {
            cin >> entry;
            if(entry.getSequence().size() == 0)
              continue;
            Matrix probabilities;
            clock_t begin = clock();
            model->decodable()->posteriorProbabilities(entry.getSequence(), probabilities);
            clock_t end = clock();
            std::cerr << "TIME: " << (double)(end - begin)/CLOCKS_PER_SEC << std::endl;
            cout << entry.getName() << endl;
            for (unsigned k = 0; k < probabilities.size1 (); k++)
                {
                    cout << states->getSymbol(k)->name() << "\t";
                    for (unsigned i = 0; i < probabilities.size2 (); i++)
                        {
                            cout << probabilities(k, i) << "\t";
                        }
                    cout << endl;
                }
    }
  }
  catch (boost::program_options::invalid_command_line_syntax &e)
  {
    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
    cout << "error: " << e.what() << std::endl;
    cout << std::endl;
    cout << desc << "\n";
  }
  catch (boost::program_options::unknown_option &e)
  {
    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
    cout << "error: " << e.what() << std::endl;
    cout << std::endl;
    cout << desc << "\n";

  }
  catch (boost::bad_any_cast & e)
  {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << "\n";
  }
  return 0;
}
