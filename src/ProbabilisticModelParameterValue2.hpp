#ifndef PROBABILISTIC_MODEL_PARAMETER_VALUE2_HPP
#define PROBABILISTIC_MODEL_PARAMETER_VALUE2_HPP

#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

namespace tops {
  class StringParameterValue2;
  class IntegerParameterValue2;
  class DoubleParameterValue2;
  class IntegerListParameterValue2;
  class DoubleListParameterValue2;
  class StringListParameterValue2;
  class ProbabilityParameterValue2;
  class IdentifierParameterValue2;
  class ProbabilisticModelConfiguration;

  class ProbabilisticModelParameterValue2 {
  public:
    virtual std::string parameterType();
    virtual std::string str();

    virtual StringParameterValue2* toStringParameter();
    virtual IntegerParameterValue2* toIntegerParameter();
    virtual DoubleParameterValue2* toDoubleParameter();
    virtual IntegerListParameterValue2* toIntegerListParameter();
    virtual DoubleListParameterValue2* toDoubleListParameter();
    virtual StringListParameterValue2* toStringListParameter();
    virtual ProbabilityParameterValue2* toProbabilityParameter();
    virtual IdentifierParameterValue2* toIdentifierParameter();
    virtual ProbabilisticModelConfiguration* toConfigurationParameter();
  };

  class StringParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    StringParameterValue2(std::string value):_value(value) {}

    std::string value();

    virtual std::string parameterType();
    virtual std::string str();
    virtual StringParameterValue2* toStringParameter();
  private:
    std::string _value;
  };

  class IntegerParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    IntegerParameterValue2(int value):_value(value) {}

    int value();

    virtual IntegerParameterValue2* toIntegerParameter();
    virtual std::string parameterType();
    virtual std::string str();
  private:
    int _value;
  };

  class DoubleParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    DoubleParameterValue2(double value):_value(value) {}

    double value();

    virtual DoubleParameterValue2* toDoubleParameter();
    virtual std::string parameterType();
    virtual std::string str();
  private:
    double _value;
  };

  class IntegerListParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    IntegerListParameterValue2(std::vector<int> value):_value(value) {}

    std::vector<int> value();

    virtual std::string parameterType();
    virtual std::string str();
  private:
    std::vector<int> _value;
  };

  class DoubleListParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    DoubleListParameterValue2(std::vector<double> value):_value(value) {}

    std::vector<double> value();

    virtual std::string parameterType();
    virtual std::string str();
  private:
    std::vector<double> _value;
  };

  class StringListParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    StringListParameterValue2(std::vector<std::string> value):_value(value) {}

    std::vector<std::string> value();

    virtual std::string parameterType();
    virtual std::string str();
  private:
    std::vector<std::string> _value;
  };

  class ProbabilityParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    ProbabilityParameterValue2(std::map<std::string, double> value):_value(value) {}

    std::map<std::string, double> value();

    virtual std::string parameterType();
  private:
    std::map<std::string, double> _value;
  };

  class IdentifierParameterValue2: public ProbabilisticModelParameterValue2 {
  public:
    IdentifierParameterValue2(std::string value):_value(value) {}

    std::string value();

    virtual std::string parameterType();
  private:
    std::string _value;
  };

  typedef boost::shared_ptr <ProbabilisticModelParameterValue2> ProbabilisticModelParameterValue2Ptr;
  typedef std::map<std::string, ProbabilisticModelParameterValue2Ptr> ProbabilisticModelParameterMap;
  typedef boost::shared_ptr <StringParameterValue2> StringParameterValue2Ptr;
  typedef boost::shared_ptr <IntegerParameterValue2> IntegerParameterValue2Ptr;
  typedef boost::shared_ptr <DoubleParameterValue2> DoubleParameterValue2Ptr;
  typedef boost::shared_ptr <IntegerListParameterValue2> IntegerListParameterValue2Ptr;
  typedef boost::shared_ptr <DoubleListParameterValue2> DoubleListParameterValue2Ptr;
  typedef boost::shared_ptr <StringListParameterValue2> StringListParameterValue2Ptr;
  typedef boost::shared_ptr <ProbabilityParameterValue2> ProbabilityParameterValue2Ptr;
  typedef boost::shared_ptr <IdentifierParameterValue2> IdentifierParameterValue2Ptr;
}

#endif
