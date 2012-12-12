#include "ProbabilisticModelParameterValue2.hpp"

#include <iostream>

namespace tops {

  // ProbabilisticModelParameterValue2
  std::string ProbabilisticModelParameterValue2::parameterType() {
    return "ProbabilisticModelParameterValue2";
  }
  StringParameterValue2* ProbabilisticModelParameterValue2::toStringParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  IntegerParameterValue2* ProbabilisticModelParameterValue2::toIntegerParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  DoubleParameterValue2* ProbabilisticModelParameterValue2::toDoubleParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  IntegerListParameterValue2* ProbabilisticModelParameterValue2::toIntegerListParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  DoubleListParameterValue2* ProbabilisticModelParameterValue2::toDoubleListParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  StringListParameterValue2* ProbabilisticModelParameterValue2::toStringListParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  ProbabilityParameterValue2* ProbabilisticModelParameterValue2::toProbabilityParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  IdentifierParameterValue2* ProbabilisticModelParameterValue2::toIdentifierParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  ProbabilisticModelConfiguration* ProbabilisticModelParameterValue2::toConfigurationParameter() {
    std::cerr << "Invalid conversion" << std::endl;
    return NULL;
  }

  // StringParameterValue2
  std::string StringParameterValue2::parameterType() {
    return "StringParameterValue2";
  }

  std::string StringParameterValue2::value() {
    return _value;
  }

  StringParameterValue2* StringParameterValue2::toStringParameter() {
    return this;
  }

  // IntegerParameterValue2
  std::string IntegerParameterValue2::parameterType() {
    return "IntegerParameterValue2";
  }

  int IntegerParameterValue2::value() {
    return _value;
  }

  // DoubleParameterValue2
  std::string DoubleParameterValue2::parameterType() {
    return "DoubleParameterValue2";
  }

  double DoubleParameterValue2::value() {
    return _value;
  }

  // IntegerListParameterValue2
  std::string IntegerListParameterValue2::parameterType() {
    return "IntegerListParameterValue2";
  }

  std::vector<int> IntegerListParameterValue2::value() {
    return _value;
  }

  // DoubleListParameterValue2
  std::string DoubleListParameterValue2::parameterType() {
    return "DoubleListParameterValue2";
  }

  std::vector<double> DoubleListParameterValue2::value() {
    return _value;
  }

  // StringListParameterValue2
  std::string StringListParameterValue2::parameterType() {
    return "StringListParameterValue2";
  }

  std::vector<std::string> StringListParameterValue2::value() {
    return _value;
  }

  // ProbabilityParameterValue2
  std::string ProbabilityParameterValue2::parameterType() {
    return "ProbabilityParameterValue2";
  }

  std::map<std::string, double> ProbabilityParameterValue2::value() {
    return _value;
  }

  // IdentifierParameterValue2
  std::string IdentifierParameterValue2::parameterType() {
    return "IdentifierParameterValue2";
  }

  std::string IdentifierParameterValue2::value() {
    return _value;
  }
}
