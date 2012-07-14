/*
 *       train.cpp
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
#include <vector>
#include <string>

#include "TrainHMMBaumWelch.hpp"
#include "TrainPHMMBaumWelch.hpp"
#include "TrainVariableLengthMarkovChain.hpp"
#include "TrainDiscreteIIDModel.hpp"
#include "TrainFixedLengthMarkovChain.hpp"
#include "TrainGHMMTransitions.hpp"
#include "TrainWeightArrayModel.hpp"
#include "BayesianInformationCriteria.hpp"
#include "AkaikeInformationCriteria.hpp"
#include "TrainVariableLengthInhomogeneousMarkovChain.hpp"
#include "SmoothedHistogramKernelDensity.hpp"
#include "SmoothedHistogramStanke.hpp"
#include "SmoothedHistogramBurge.hpp"
#include "TrainPhasedMarkovChain.hpp"
#include "TrainInterpolatedPhasedMarkovChain.hpp"
#include "TrainInterpolatedMarkovChain.hpp"
#include "TrainSimilarityBasedSequenceWeighting.hpp"
#include "TrainPhasedMarkovChainContextAlgorithm.hpp"
#include "TrainHMMMaximumLikelihood.hpp"
#include "RemoveSequenceFromModel.hpp"
#include "SequenceFormat.hpp"
#include "version.hpp"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>

using namespace tops;
using namespace std;
using namespace boost::program_options;

int main(int argc, char ** argv) {

        options_description desc("Allowed options");
        desc.add_options()
          ("help,h", "produce help message")
          ("configuration,c",value<string> (), "configuration file")
          ("output,o",value<string> (), "output file")
          ("fasta,F",  "use fasta format");

        map<string, ProbabilisticModelCreatorPtr> createModelCommand;
        map<string, ProbabilisticModelCreatorPtr> modelSelectionCommand;
        map<string, ProbabilisticModelCreatorPtr> decoratorCommand;
        createModelCommand["ContextAlgorithm"] = TrainVariableLengthMarkovChainPtr(
                        new TrainVariableLengthMarkovChain());
        createModelCommand["GHMMTransitions"] = TrainGHMMTransitionsCreatorPtr(
                        new TrainGHMMTransitionsCreator());
        createModelCommand["FixedLengthMarkovChain"]
                        = TrainFixedLengthMarkovChainPtr(new TrainFixedLengthMarkovChain());
        createModelCommand["BaumWelchHMM"] = TrainHMMBaumWelchPtr(
                        new TrainHMMBaumWelch());
        createModelCommand["MaximumLikelihoodHMM"] = TrainHMMMaximumLikelihoodPtr(
			new TrainHMMMaximumLikelihood());

        createModelCommand["PHMMBaumWelch"] = TrainPHMMBaumWelchPtr(
                        new TrainPHMMBaumWelch());
        createModelCommand["WeightArrayModel"] = TrainWeightArrayModelPtr(
                        new TrainWeightArrayModel());
        createModelCommand["VariableLengthInhomogeneousMarkovChain"]
                        = TrainVariableLengthInhomogeneousMarkovChainPtr(
                                        new TrainVariableLengthInhomogeneousMarkovChain());
        createModelCommand["PhasedMarkovChain"] = TrainPhasedMarkovChainPtr(
                        new TrainPhasedMarkovChain());
        createModelCommand["InterpolatedPhasedMarkovChain"] = TrainInterpolatedPhasedMarkovChainPtr(
                        new TrainInterpolatedPhasedMarkovChain());

        createModelCommand["InterpolatedMarkovChain"] = TrainInterpolatedMarkovChainPtr(
                        new TrainInterpolatedMarkovChain());


        createModelCommand["SBSW"] = TrainSimilarityBasedSequenceWeightingPtr(
                        new TrainSimilarityBasedSequenceWeighting());

        createModelCommand["PhasedMarkovChainContextAlgorithm"] = TrainPhasedMarkovChainContextAlgorithmPtr(
                                                                                                            new TrainPhasedMarkovChainContextAlgorithm());

        createModelCommand["SmoothedHistogramKernelDensity"] = SmoothedHistogramKernelDensityPtr(new SmoothedHistogramKernelDensity());
        createModelCommand["SmoothedHistogramStanke"] = SmoothedHistogramStankePtr(new SmoothedHistogramStanke());
        createModelCommand["SmoothedHistogramBurge"] = SmoothedHistogramBurgePtr(new SmoothedHistogramBurge());
        createModelCommand["DiscreteIIDModel"] = TrainDiscreteIIDModelPtr(new TrainDiscreteIIDModel());


        modelSelectionCommand["BIC"] = BayesianInformationCriteriaPtr(
                        new BayesianInformationCriteria());
        modelSelectionCommand["AIC"] = AkaikeInformationCriteriaPtr(
                        new AkaikeInformationCriteria());

        decoratorCommand["RemoveSequence"] = RemoveSequenceFromModelPtr(new RemoveSequenceFromModel());
        try {
                variables_map vm;
                store(parse_command_line(argc, argv, desc), vm);
                notify(vm);
                if (vm.count("fasta"))
                  SequenceFormatManager::instance()->setFormat(FastaSequenceFormatPtr(new FastaSequenceFormat()));
                if (vm.count("help")) {
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;

                  cerr << desc << "\n";
                  cerr << "Implemented algorithms are: " << endl;
                  map<string, ProbabilisticModelCreatorPtr>::iterator it;
                  for (it = createModelCommand.begin(); it
                         != createModelCommand.end(); it++)
                    cerr << "\t" << it->first << endl;
                  exit(0);
                  return 1;
                }

                if (vm.count("configuration")) {
                        string file = vm["configuration"].as<string> ();
                        ConfigurationReader readConfig;

                        std::ifstream input;
                        std::string line;

                        input.open(file.c_str());
                        if (!input.is_open()) {
                                std::cerr << "Cant open file " << file << std::endl;
                                exit(0);
                        }
                        string conf;
                        while (!input.eof()) {
                                getline(input, line, '\n');
                                line += "\n";
                                conf.append(line);
                        }
                        input.close();
                        if (readConfig.load(conf)) {

                                ProbabilisticModelParameterValuePtr create_model =
                                  (readConfig.parameters())->getMandatoryParameterValue(
                                                                "training_algorithm");
                                ProbabilisticModelParameterValuePtr bic =
                                  (readConfig.parameters())->getOptionalParameterValue(
                                                                "model_selection_criteria");

                                ProbabilisticModelParameterValuePtr decorator =
                                  (readConfig.parameters())->getOptionalParameterValue(
                                                                "decorator");

                                if (create_model == NULL) {
                                        exit(0);
                                }

                                string command = create_model->getString();

                                ProbabilisticModelCreatorPtr creator;
                                if (createModelCommand.find(command)
                                                == createModelCommand.end()) {
                                        cerr << "ERROR: invalid  training algorithm: " << command
                                                        << endl;
                                        cerr << "Implemented training algorithms are: " << endl;
                                        map<string, ProbabilisticModelCreatorPtr>::iterator it;
                                        for (it = createModelCommand.begin(); it
                                                        != createModelCommand.end(); it++)
                                                cerr << "\t" << it->first << endl;
                                        exit(0);
                                } else
                                        creator = createModelCommand[command];

                                if (bic != NULL) {
                                        if (modelSelectionCommand.find(bic->getString())
                                                        != modelSelectionCommand.end()) {
                                                creator = modelSelectionCommand[bic->getString()];
                                                creator->setCreator(createModelCommand[command]);
                                        } else {
                                                cerr << "ERROR: invalid  model selection criteria: "
                                                                << command << endl;
                                                cerr << "Implemented model selection are: " << endl;
                                                map<string, ProbabilisticModelCreatorPtr>::iterator it;
                                                for (it = modelSelectionCommand.begin(); it
                                                                != modelSelectionCommand.end(); it++)
                                                        cerr << "\t" << it->first << endl;
                                                exit(0);
                                        }

                                }

                                if(decorator != NULL) {
                                  if (decoratorCommand.find(decorator->getString())
                                      != decoratorCommand.end()) {
                                    decoratorCommand[decorator->getString()]->setCreator(creator);
                                    creator = decoratorCommand[decorator->getString()];
                                  } else {
                                    cerr << "ERROR: invalid  decorator: "
                                         << command << endl;
                                    cerr << "Implemented decorators are: " << endl;
                                    map<string, ProbabilisticModelCreatorPtr>::iterator it;
                                    for (it = decoratorCommand.begin(); it
                                           != decoratorCommand.end(); it++)
                                      cerr << "\t" << it->first << endl;
                                    exit(0);
                                  }


                                }
                                struct timeval start, stop;
                                clock_t begin = clock();
                                ProbabilisticModelPtr model = creator->create( *(readConfig.parameters()));
                                clock_t end = clock();
                                std::cerr << "TIME: " << (double)(end - begin)/CLOCKS_PER_SEC << std::endl;
                                if(model == NULL) {
                                    std::cerr << "ERROR: Could not create model !" << std::endl;
                                    exit(-1);
                                }
                                if (vm.count("output")) {
                                        string file = vm["output"].as<string> ();
                                        ofstream output(file.c_str());
                                        output << model->str() << std::endl;
                                        output.close();
                                } else
                                        std::cout << model->str() << std::endl;
                        }
                } else {
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << desc << "\n";

                        exit(0);
                }
        } catch (boost::program_options::invalid_command_line_syntax &e) {
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << "error: " << e.what() << std::endl;
                    cout << desc << "\n";
        } catch (boost::program_options::unknown_option &e) {
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << "error: " << e.what() << std::endl;
                    cout << desc << "\n";
        } catch (boost::bad_any_cast & e) {
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << desc << "\n";
        }

        return 0;
}
