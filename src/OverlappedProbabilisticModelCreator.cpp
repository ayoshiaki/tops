/*
 *       OverlappedProbabilisticModelCreator.cpp
 *
 *       Copyright 2011 Waldir Caro <waldirc@ime.usp.br>
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

#include "OverlappedProbabilisticModelCreator.hpp"
#include "SequenceFactory.hpp"
#include "SequenceFactory.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "OverlappedProbabilisticModel.hpp"

namespace tops
{ 

  ProbabilisticModelPtr OverlappedProbabilisticModelCreator::create(ProbabilisticModelParameters &parameters) const
  {
    ProbabilisticModelParameterValuePtr model_par = parameters.getMandatoryParameterValue("model");
    ProbabilisticModelParameterValuePtr left_overlap_par = parameters.getMandatoryParameterValue("left_overlap");
    ProbabilisticModelParameterValuePtr rigth_overlap_par = parameters.getMandatoryParameterValue("rigth_overlap");
    ProbabilisticModelParameterValuePtr signal_size_par = parameters.getMandatoryParameterValue("signal_size");

    ProbabilisticModelParameterValuePtr reverse_par = parameters.getOptionalParameterValue("reverse");

    std::cerr << "[INFO] Overlapped models: " << model_par->getString() << std::endl;
    if(model_par == NULL || left_overlap_par == NULL || rigth_overlap_par == NULL || signal_size_par == NULL) {
        std::cerr << "[INFO] Overlapped parameters" << std::endl;
        std::cerr << help() << std::endl;
        return NULL;
    }

    int left_overlap = left_overlap_par->getInt();
    int rigth_overlap = rigth_overlap_par->getInt();
    int signal_size = signal_size_par->getInt();
    int reverse = reverse_par->getInt();
    std::string modelstr = model_par->getString();
    std::string modelstr_aux = "";

    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    ProbabilisticModelPtr m;

    if ((modelstr.size()) > 0 && (modelstr[0] == '[')) // load a config specification (e.g. NeuralNetwork)
    {
      modelstr = modelstr.substr(1, modelstr.size() - 2);
      if (reader.load(modelstr))
      {        
        ProbabilisticModelParametersPtr par = reader.parameters();
        m = creator.create(*par);
      }
      else // load a file containing the model
      {
        std::cerr << "Can not load model config /=======/\n"
                  << modelstr << "/========/" << std::endl;
        exit(-1);
      }
    }
    else
    {      
      m = creator.create(modelstr);
      modelstr_aux = modelstr;
      if (m == NULL)
      {
        std::cerr << "Can not load model file " << modelstr << "! " << std::endl;
        exit(-1);
      }
    }    

    // create the decorator of our signal model (m is a NeuralNetwork)
    OverlappedProbabilisticModelPtr decorator = OverlappedProbabilisticModelPtr(new OverlappedProbabilisticModel(m));

    decorator->setSubModel(m);
    decorator->subModelName(modelstr_aux);    
    decorator->initialize(left_overlap, rigth_overlap, signal_size, reverse);
    decorator->setAlphabet(m->alphabet());

    return decorator;
  }

  ProbabilisticModelPtr OverlappedProbabilisticModelCreator::create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
    ProbabilisticModelParameterValuePtr model_par = parameters.getMandatoryParameterValue("model");
    ProbabilisticModelParameterValuePtr left_overlap_par = parameters.getMandatoryParameterValue("left_overlap");
    ProbabilisticModelParameterValuePtr rigth_overlap_par = parameters.getMandatoryParameterValue("rigth_overlap");
    ProbabilisticModelParameterValuePtr signal_size_par = parameters.getMandatoryParameterValue("signal_size"); 
    ProbabilisticModelParameterValuePtr reverse_par = parameters.getOptionalParameterValue("reverse");

    std::cerr << "[INFO] Overlapped models: " << model_par->getString() << std::endl;
    if(model_par == NULL || left_overlap_par == NULL || rigth_overlap_par == NULL || signal_size_par == NULL) {
        std::cerr << help() << std::endl;
        return NULL;
    }

    int left_overlap = left_overlap_par->getInt();
    int rigth_overlap = rigth_overlap_par->getInt();
    int signal_size = signal_size_par->getInt();
    int reverse = reverse_par->getInt();
    std::string modelstr = model_par->getString();
    std::string modelstr_aux = "";

    /*ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    ProbabilisticModelPtr m;

    if ((modelstr.size()) > 0 && (modelstr[0] == '[')) // load a config specification (e.g. NeuralNetwork)
    {
      modelstr = modelstr.substr(1, modelstr.size() - 2);
      if (reader.load(modelstr))
      {        
        ProbabilisticModelParametersPtr par = reader.parameters();
        m = creator.create(*par);
      }
      else // load a file containing the model
      {
        std::cerr << "Can not load model config /=======/\n"
                  << modelstr << "/========/" << std::endl;
        exit(-1);
      }
    }
    else
    {   
      std::cerr << "\t[INFO] OVERLAPPED CALLING init open file creator: "  << modelstr << std::endl;
      m = creator.create(modelstr);
      modelstr_aux = modelstr;
      if (m == NULL)
      {
        std::cerr << "Can not load model file " << modelstr << "! " << std::endl;
        exit(-1);
      }
    }*/
    ConfigurationReader reader;
    ProbabilisticModelPtr m;
    std::map<std::string, ProbabilisticModelPtr>::const_iterator it = models.find(modelstr);
    if(it != models.end()){
      std::cerr << "\t[INFO] OVERLAPPED was called: " << modelstr << std::endl;
      m = it->second;
      modelstr_aux = modelstr;
    }
    else{
      std::cerr << "Model " << modelstr << " not loaded ! " << std::endl;
      std::exit(-1);
    }    

    // create the decorator of our signal model (m is a NeuralNetwork)
    OverlappedProbabilisticModelPtr decorator = OverlappedProbabilisticModelPtr(new OverlappedProbabilisticModel(m));

    decorator->setSubModel(m);
    decorator->subModelName(modelstr_aux);    
    decorator->initialize(left_overlap, rigth_overlap, signal_size, reverse);
    decorator->setAlphabet(m->alphabet());

    return decorator;
  }

}
