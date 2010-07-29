#ifndef PROBABILISTIC_MODEL_CREATOR_CLIENT_HPP
#define PROBABILISTIC_MODEL_CREATOR_CLIENT_HPP


#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelParameter.hpp"

namespace tops {
  class ProbabilisticModelCreatorClient {
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
