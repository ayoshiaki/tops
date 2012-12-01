/*
 *       ConfigurationReader.hpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
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

#ifndef CONFIGURATION_READER_HPP
#define CONFIGURATION_READER_HPP
#include <boost/shared_ptr.hpp>

#include "crossplatform.hpp"

#include <iostream>
#include <string>


using namespace std;


namespace tops {

  class DLLEXPORT Alphabet;
  class DLLEXPORT ProbabilisticModelParameters;

  class DLLEXPORT ProbabilisticModelParameterValue;
  typedef boost::shared_ptr<ProbabilisticModelParameterValue> ProbabilisticModelParameterValuePtr;
  typedef boost::shared_ptr<ProbabilisticModelParameters> ProbabilisticModelParametersPtr;

  //! This class  reads a configuration file
  class DLLEXPORT ConfigurationReader {
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
