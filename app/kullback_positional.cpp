/*
 *       kullback_positional.cpp
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
#include "TrainWeightArrayModel.hpp"
#include "ConfigurationReader.hpp"
#include "InhomogeneousFactorableModel.hpp"
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
    ("positive,p", value<string> (), "positive sequences")
    ("negative,n", value<string> (), "negative sequences")
    ("length,l", value<int> (), "length of the window" )
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
          return 1;

      }
      if(vm.count("negative")<= 0) {
          cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
          cout << std::endl;
          cout << desc << "\n";
          return 1;
      }
      if(vm.count("length")<= 0) {
        cout << "ERROR: Length of the window is a mandatory parameter" << std::endl;
        cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
        cout << std::endl;
        cout << desc << "\n";
        return 1;
      }

      if (vm.count("fasta"))
        SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));



      TrainWeightArrayModelPtr creator = TrainWeightArrayModelPtr(new TrainWeightArrayModel());
      ProbabilisticModelParameters training_parameters;
      std::string positive_set = vm["positive"].as<string> ();
      std::string negative_set = vm["negative"].as<string> ();
      int length = vm["length"].as<int>();
      AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
      alphabet->createSymbol("A");
      alphabet->createSymbol("C");
      alphabet->createSymbol("G");
      alphabet->createSymbol("T");
      ProbabilisticModelParameterValuePtr alphabetpar = alphabet->getParameterValue();
      ProbabilisticModelParameterValuePtr trainingsetpar = StringParameterValuePtr(new StringParameterValue(positive_set));
      ProbabilisticModelParameterValuePtr trainingsetpar2 = StringParameterValuePtr(new StringParameterValue(negative_set));
      ProbabilisticModelParameterValuePtr orderpar =IntParameterValuePtr(new IntParameterValue(0));
      ProbabilisticModelParameterValuePtr lengthpar = IntParameterValuePtr(new IntParameterValue(length));
      training_parameters.add("alphabet", alphabetpar);
      training_parameters.add("training_set", trainingsetpar);
      training_parameters.add("order", orderpar);
      training_parameters.add("length", lengthpar);
      ProbabilisticModelParameters training_parameters2;
      training_parameters2.add("alphabet", alphabetpar);
      training_parameters2.add("training_set", trainingsetpar2);
      training_parameters2.add("order", orderpar);
      training_parameters2.add("length", lengthpar);

      ProbabilisticModelPtr positiveModel = creator->create(training_parameters);
      ProbabilisticModelPtr negativeModel = creator->create(training_parameters2);

      std::cout << "position" <<"\t" << "kl" << "\t" << "entropy"<< std::endl;
      for(int i = 0; i < length; i++)
        {
          double kl = 0;
          double entropy = 0;
          for(int j = 0; j < 4; j++)
            {
              Sequence s;
              s.push_back(j);
              double p = exp(positiveModel->inhomogeneous()->evaluatePosition(s, 0, i));
              double q = exp(negativeModel->inhomogeneous()->evaluatePosition(s, 0, i));
              kl += p * logf(p/q)/logf(2);
              entropy += p * logf(p)/logf(2);
            }
          std::cout << i << "\t" << kl << "\t" << entropy << std::endl;
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
