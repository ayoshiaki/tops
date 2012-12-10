/*
 *       ProbabilisticModelCreatorClient.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bon�dio <ibonadio@ime.usp.br>
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

#include "ProbabilisticModelCreatorClient.hpp"
#include "StoreLoadedModel.hpp"

#include "ProbabilisticModelCreator.hpp"
#include "DiscreteIIDModelCreator.hpp"
#include "BernoulliModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "InhomogeneousMarkovChainCreator.hpp"
#include "HiddenMarkovModelCreator.hpp"
#include "PairHiddenMarkovModelCreator.hpp"
#include "ProfileHiddenMarkovModelCreator.hpp"
#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "TargetModelCreator.hpp"
#include "SmoothedHistogramKernelDensity.hpp"
#include "FixedSequenceAtPositionCreator.hpp"
#include "ReverseComplementDNACreator.hpp"
#include "PhasedRunLengthDistributionCreator.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "SimilarityBasedSequenceWeightingCreator.hpp"
#include "MultipleSequentialModelCreator.hpp"
#include "util.hpp"


#include "TrainHMMBaumWelch.hpp"
#include "TrainProfileHMMMaxLikelihood.hpp"
#include "TrainVariableLengthMarkovChain.hpp"
#include "TrainDiscreteIIDModel.hpp"
#include "TrainFixedLengthMarkovChain.hpp"
#include "TrainWeightArrayModel.hpp"
#include "BayesianInformationCriteria.hpp"
#include "AkaikeInformationCriteria.hpp"
#include "TrainVariableLengthInhomogeneousMarkovChain.hpp"
#include "SmoothedHistogramKernelDensity.hpp"
#include "SmoothedHistogramStanke.hpp"
#include "SmoothedHistogramBurge.hpp"
#include "TrainPhasedMarkovChain.hpp"
#include "TrainInterpolatedPhasedMarkovChain.hpp"
#include "TrainPhasedMarkovChainContextAlgorithm.hpp"
#include "RemoveSequenceFromModel.hpp"
#include "SequenceFormat.hpp"


namespace tops
{

  ProbabilisticModelPtr ProbabilisticModelCreatorClient::create(ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models)
  {
    ProbabilisticModelParameterValuePtr modelnamepar = parameters.getMandatoryParameterValue("model_name");
    if(modelnamepar == NULL)
      {
        std::cerr << "Cant create model with no name !" << std::endl;
        exit(-1);
      }
    string command = modelnamepar->getString();
    if(_createModelCommand.find(command) == _createModelCommand.end())
      {
        cerr << "ERROR: invalid  model: " << command << endl;
        cerr << "Implemented model are: " << endl;
        map<string, ProbabilisticModelCreatorPtr>::iterator it;
        for(it = _createModelCommand.begin(); it != _createModelCommand.end(); it++)
          cerr << "\t" << it->first << endl;
        exit(-1);
      }
    ProbabilisticModelPtr m = _createModelCommand[command]->create(parameters, models);
    return m;
  }




  ProbabilisticModelPtr ProbabilisticModelCreatorClient::create(ProbabilisticModelParameters & parameters)
  {
    ProbabilisticModelPtr null;
    ProbabilisticModelParameterValuePtr modelnamepar = parameters.getMandatoryParameterValue("model_name");
    if(modelnamepar == NULL)
      {
        std::cerr << "Cant create model with no name !" << std::endl;
        return null;
      }
    string command = modelnamepar->getString();
    if(_createModelCommand.find(command) == _createModelCommand.end())
      {
        cerr << "ERROR: invalid  model: " << command << endl;
        cerr << "Implemented model are: " << endl;
        map<string, ProbabilisticModelCreatorPtr>::iterator it;
        for(it = _createModelCommand.begin(); it != _createModelCommand.end(); it++)
          cerr << "\t" << it->first << endl;

        return null;
      }
    ProbabilisticModelPtr m = _createModelCommand[command]->create(parameters);
    return m;
  }

  ProbabilisticModelPtr ProbabilisticModelCreatorClient::train (const std::string & input_file_name)
  {
    return train(readConfigurationFromFile(input_file_name));
  }

    ProbabilisticModelPtr ProbabilisticModelCreatorClient::create(const std::string & input_file_name)
    {
        ProbabilisticModelPtr m = StoreLoadedModel::instance()->get(input_file_name);
        if(m != NULL)
            return m;
        m = create(readConfigurationFromFile(input_file_name));
        return StoreLoadedModel::instance()->add(input_file_name, m);
    }

  ProbabilisticModelParameters & ProbabilisticModelCreatorClient::readConfigurationFromFile(const std::string  & filename)
  {
    ConfigurationReader readConfig;
    std::ifstream input;
    std::string line;
    ProbabilisticModelPtr null;
    input.open(filename.c_str());
    if(!input.is_open())
      {
        std::cerr << "Cant open file "  << filename << std::endl;
        return _p;
      }
    string conf;
    while(!input.eof())
      {
        getline(input,line,'\n');
        line += "\n";
        conf.append(line);
      }
    input.close();
    if(readConfig.load(conf)){
      _p = *(readConfig.parameters());
      return _p;
    }

    std::cerr << "Error reading configuration file ! " << std::endl;
    return _p;
  }

  ProbabilisticModelPtr ProbabilisticModelCreatorClient::train (ProbabilisticModelParameters & parameters)
  {

    ProbabilisticModelParameterValuePtr create_model =
      parameters.getMandatoryParameterValue("training_algorithm");
    ProbabilisticModelParameterValuePtr bic =  parameters.getOptionalParameterValue("model_selection_criteria");
    ProbabilisticModelParameterValuePtr decorator =  parameters.getOptionalParameterValue("decorator");

    if (create_model == NULL) {
      exit(-1);
    }

    string command = create_model->getString();

    ProbabilisticModelCreatorPtr creator;
    if (_trainingCommand.find(command) == _trainingCommand.end()) {
      cerr << "ERROR: invalid  training algorithm: " << command
           << endl;
      cerr << "Implemented training algorithms are: " << endl;
      map<string, ProbabilisticModelCreatorPtr>::iterator it;
      for (it = _trainingCommand.begin(); it
             != _trainingCommand.end(); it++)
        cerr << "\t" << it->first << endl;
      exit(-1);
    } else
      creator = _trainingCommand[command];

    if (bic != NULL) {
      if (_modelSelectionCommand.find(bic->getString())
          != _modelSelectionCommand.end()) {
        creator = _modelSelectionCommand[bic->getString()];
        creator->setCreator(_trainingCommand[command]);
      } else {
        cerr << "ERROR: invalid  model selection criteria: "
             << command << endl;
        cerr << "Implemented model selection are: " << endl;
        map<string, ProbabilisticModelCreatorPtr>::iterator it;
        for (it = _modelSelectionCommand.begin(); it
               != _modelSelectionCommand.end(); it++)
          cerr << "\t" << it->first << endl;
        exit(-1);
      }

    }

    if(decorator != NULL) {
      if (_decoratorCommand.find(decorator->getString())
          != _decoratorCommand.end()) {
        _decoratorCommand[decorator->getString()]->setCreator(creator);
        creator = _decoratorCommand[decorator->getString()];
      } else {
        cerr << "ERROR: invalid  decorator: "
             << command << endl;
        cerr << "Implemented decorators are: " << endl;
        map<string, ProbabilisticModelCreatorPtr>::iterator it;
        for (it = _decoratorCommand.begin(); it
               != _decoratorCommand.end(); it++)
          cerr << "\t" << it->first << endl;
        exit(-1);
      }


    }
    ProbabilisticModelPtr model = creator->create(parameters);
#if 0
    struct timeval start, stop;
    gettimeofday(&start, (struct timezone *) NULL);
    ProbabilisticModelPtr model = creator->create(parameters);
    gettimeofday(&stop, (struct timezone *)NULL);
    stop.tv_sec -= start.tv_sec;
    stop.tv_usec -= start.tv_usec;
    if(stop.tv_usec  < 0){
      stop.tv_sec --;
      stop.tv_usec += 1000000;
    }
    fprintf(stderr, "Elapsed time %ld%c%02d seconds\n", stop.tv_sec, '.', stop.tv_usec/1000);
#endif
    return model;

  }


  void ProbabilisticModelCreatorClient::registry_new_creator(std::string name, ProbabilisticModelCreatorPtr creator){
    if (_createModelCommand.find(name) == _createModelCommand.end() )
      _createModelCommand[name] = creator;
  }
  void ProbabilisticModelCreatorClient::registry_new_training(std::string name, ProbabilisticModelCreatorPtr creator){
    if(_trainingCommand.find(name) == _trainingCommand.end())
      _trainingCommand[name] = creator;
  }
  void ProbabilisticModelCreatorClient::registry_new_model_selector(std::string name, ProbabilisticModelCreatorPtr creator){
    if(_modelSelectionCommand.find(name) == _modelSelectionCommand.end())
      _modelSelectionCommand[name] = creator;
  }
  void ProbabilisticModelCreatorClient::registry_new_decorator(std::string name, ProbabilisticModelCreatorPtr creator){
    if(_decoratorCommand.find(name) == _decoratorCommand.end())
      _decoratorCommand[name] = creator;
  }


  ProbabilisticModelCreatorClient::  ProbabilisticModelCreatorClient() {
    _trainingCommand["ContextAlgorithm"] = TrainVariableLengthMarkovChainPtr(new TrainVariableLengthMarkovChain());
    _trainingCommand["FixedLengthMarkovChain"]= TrainFixedLengthMarkovChainPtr(new TrainFixedLengthMarkovChain());
    _trainingCommand["BaumWelch"] = TrainHMMBaumWelchPtr(new TrainHMMBaumWelch());
    _trainingCommand["ProfileHMMMaxLikelihood"] = TrainProfileHMMMaxLikelihoodPtr(new TrainProfileHMMMaxLikelihood());
    _trainingCommand["WeightArrayModel"] = TrainWeightArrayModelPtr(new TrainWeightArrayModel());
    _trainingCommand["VariableLengthInhomogeneousMarkovChain"]
      = TrainVariableLengthInhomogeneousMarkovChainPtr(new TrainVariableLengthInhomogeneousMarkovChain());
    _trainingCommand["PhasedMarkovChain"] = TrainPhasedMarkovChainPtr(new TrainPhasedMarkovChain());
    _trainingCommand["InterpolatedPhasedMarkovChain"] = TrainInterpolatedPhasedMarkovChainPtr(new TrainInterpolatedPhasedMarkovChain());
    _trainingCommand["PhasedMarkovChainContextAlgorithm"] = TrainPhasedMarkovChainContextAlgorithmPtr(new TrainPhasedMarkovChainContextAlgorithm());
    _trainingCommand["SmoothedHistogramKernelDensity"] = SmoothedHistogramKernelDensityPtr(new SmoothedHistogramKernelDensity());
    _trainingCommand["SmoothedHistogramStanke"] = SmoothedHistogramStankePtr(new SmoothedHistogramStanke());
    _trainingCommand["SmoothedHistogramBurge"] = SmoothedHistogramBurgePtr(new SmoothedHistogramBurge());
    _trainingCommand["DiscreteIIDModel"] = TrainDiscreteIIDModelPtr(new TrainDiscreteIIDModel());
    _trainingCommand["MultinomialDistribution"] = TrainDiscreteIIDModelPtr(new TrainDiscreteIIDModel());
    _modelSelectionCommand["BIC"] = BayesianInformationCriteriaPtr(new BayesianInformationCriteria());
    _modelSelectionCommand["AIC"] = AkaikeInformationCriteriaPtr(new AkaikeInformationCriteria());
    _decoratorCommand["RemoveSequence"] = RemoveSequenceFromModelPtr(new RemoveSequenceFromModel());
    _createModelCommand["DiscreteIIDModel"] =
      DiscreteIIDModelCreatorPtr(new DiscreteIIDModelCreator());
    _createModelCommand["MultinomialDistribution"] =
      DiscreteIIDModelCreatorPtr(new DiscreteIIDModelCreator());
    _createModelCommand["VariableLengthMarkovChain"] =
      VariableLengthMarkovChainCreatorPtr(new VariableLengthMarkovChainCreator());
    _createModelCommand["InhomogeneousMarkovChain"] =
      InhomogeneousMarkovChainCreatorPtr(new InhomogeneousMarkovChainCreator());
    _createModelCommand["HiddenMarkovModel"] =
      HiddenMarkovModelCreatorPtr(new HiddenMarkovModelCreator());
    _createModelCommand["PairHiddenMarkovModel"] =
      PairHiddenMarkovModelCreatorPtr(new PairHiddenMarkovModelCreator());
    _createModelCommand["ProfileHiddenMarkovModel"] =
      ProfileHiddenMarkovModelCreatorPtr(new ProfileHiddenMarkovModelCreator());
    _createModelCommand["GeneralizedHiddenMarkovModel"] =
      GeneralizedHiddenMarkovModelCreatorPtr(new GeneralizedHiddenMarkovModelCreator());
    _createModelCommand["Bernoulli"] =
      BernoulliModelCreatorPtr(new BernoulliModelCreator());
    _createModelCommand["TargetModel"] =
      TargetModelCreatorPtr(new TargetModelCreator());
    _createModelCommand["FixedSequenceAtPosition"] =
      FixedSequenceAtPositionCreatorPtr(new FixedSequenceAtPositionCreator());
    _createModelCommand["PhasedRunLengthDistribution"] =
      PhasedRunLengthDistributionCreatorPtr(new PhasedRunLengthDistributionCreator());
    _createModelCommand["ReverseComplementDNA"] =
      ReverseComplementDNACreatorPtr(new ReverseComplementDNACreator());
    _createModelCommand["SimilarityBasedSequenceWeighting"] =
      SimilarityBasedSequenceWeightingCreatorPtr(new SimilarityBasedSequenceWeightingCreator());
    _createModelCommand["MultipleSequentialModels"] = MultipleSequentialModelCreatorPtr( new MultipleSequentialModelCreator());

  }
}

