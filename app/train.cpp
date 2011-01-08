/*
 *       train.cpp
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
#include <vector>
#include <string>

#include "TrainHMMBaumWelch.hpp"
#include "TrainVariableLengthMarkovChain.hpp"
#include "TrainMultinomialDistribution.hpp"
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
#include "RemoveSequenceFromModel.hpp"
#include "SequenceFormat.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
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
	createModelCommand["BaumWelch"] = TrainHMMBaumWelchPtr(
			new TrainHMMBaumWelch());
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
	createModelCommand["MultinomialDistribution"] = TrainMultinomialDistributionPtr(new TrainMultinomialDistribution());

	
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
		  cerr << desc << "\n";
		  cerr << "Implemented algorithms are: " << endl;
		  map<string, ProbabilisticModelCreatorPtr>::iterator it;
		  for (it = createModelCommand.begin(); it
			 != createModelCommand.end(); it++)
		    cerr << "\t" << it->first << endl;
		  exit(-1);
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
				exit(-1);
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
					exit(-1);
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
					exit(-1);
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
						exit(-1);
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
				    exit(-1);
				  }
				  
				  
				}
				struct timeval start, stop;
				gettimeofday(&start, (struct timezone *) NULL);
				ProbabilisticModelPtr model = creator->create( *(readConfig.parameters()));
				gettimeofday(&stop, (struct timezone *)NULL);
				stop.tv_sec -= start.tv_sec;
				stop.tv_usec -= start.tv_usec;
				if(stop.tv_usec  < 0){
				  stop.tv_sec --;
				  stop.tv_usec += 1000000;
				}
				fprintf(stderr, "Elapsed time %ld%c%02d seconds\n", stop.tv_sec, '.', stop.tv_usec/1000);

				if (vm.count("output")) {
					string file = vm["output"].as<string> ();
					ofstream output(file.c_str());
					output << model->str() << std::endl;
					output.close();
				} else
					std::cout << model->str() << std::endl;
			}
		} else {
			cerr << desc << endl;
			exit(-1);
		}
	} catch (boost::program_options::invalid_command_line_syntax &e) {
		std::cerr << "error: " << e.what() << std::endl;
		cerr << desc << endl;
	} catch (boost::program_options::unknown_option &e) {
		std::cerr << "error: " << e.what() << std::endl;
		cerr << desc << endl;
	} catch (boost::bad_any_cast & e) {
		cerr << desc << endl;
	}

	return 0;
}
