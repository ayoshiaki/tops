#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string> 


using namespace std;


namespace tops {

  class Alphabet;
  class ProbabilisticModelParameters;

  class ProbabilisticModelParameterValue;
  typedef boost::shared_ptr<ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  typedef boost::shared_ptr<ProbabilisticModelParameters> ProbabilisticModelParametersPtr;

  //! This class is reads a configuration file
  class ConfigurationReader {
  public: 

    ConfigurationReader();
    //! Loads the configuration from the data string
    bool load( std::string & data);

    //! Loads the configuration from a file
    bool loadFromFile( const std::string & filename);

    void setCurrentParameterValue(ProbabilisticModelParameterValuePtr value);

    ProbabilisticModelParameterValuePtr getCurrentParameterValue();

    void setCurrentParameterName(const std::string & name);

    void setAuxString(const std::string & aux) ;

    std::string getAuxString();

    std::string getCurrentParameterName();

    void add_parameter();

    ProbabilisticModelParametersPtr parameters();

    std::string getAuxString2();

    std::string getAuxString3();
    
    void setAuxString2(const std::string & aux);

    void setAuxString3(const std::string & aux);
    void reset();

  private:
    ProbabilisticModelParametersPtr _parameters;
    ProbabilisticModelParameterValuePtr _current_value;
    std::string  _current_name;
    std::string _aux_string;
    std::string _aux_string_2;
    std::string _aux_string_3;
  };


  
}

#endif
