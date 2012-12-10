/*
 *       ProbabilisticModelCreatorClient.hpp
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

#ifndef PROBABILISTIC_MODEL_CREATOR_CLIENT_HPP
#define PROBABILISTIC_MODEL_CREATOR_CLIENT_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelParameter.hpp"

namespace tops {
  //! Creates a new probabilistic model
  class DLLEXPORT ProbabilisticModelCreatorClient {
  public:
    ProbabilisticModelCreatorClient();
    ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters);
    ProbabilisticModelPtr create(ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models);
    ProbabilisticModelPtr create(const std::string & input_file_name);
    ProbabilisticModelPtr train (ProbabilisticModelParameters & parameters);
    ProbabilisticModelPtr train (const std::string & input_file_name);

    void registry_new_creator(std::string name, ProbabilisticModelCreatorPtr creator);
    void registry_new_training(std::string name, ProbabilisticModelCreatorPtr creator);
    void registry_new_model_selector(std::string name, ProbabilisticModelCreatorPtr creator);
    void registry_new_decorator(std::string name, ProbabilisticModelCreatorPtr creator);

  private:
    std::map<std::string, ProbabilisticModelCreatorPtr> _createModelCommand;
    std::map<std::string, ProbabilisticModelCreatorPtr> _trainingCommand;
    std::map<std::string, ProbabilisticModelCreatorPtr> _modelSelectionCommand;
    std::map<std::string, ProbabilisticModelCreatorPtr> _decoratorCommand;
    ProbabilisticModelParameters & readConfigurationFromFile(const std::string  & filename);
    ProbabilisticModelParameters _p;
  };
}
#endif
