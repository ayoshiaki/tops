/*
 *       ProbabilisticModelParameter.cpp
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

#include "ProbabilisticModelParameter.hpp"
#include "util.hpp"

namespace tops {

  void ProbabilisticModelParameters::add(const char * name, ProbabilisticModelParameterValuePtr value) {
    std::string s(name);
    add(s, value);
  }

  //! Adds a new model parameter
  void ProbabilisticModelParameters::add(std::string name, ProbabilisticModelParameterValuePtr value)
  {
    if(_parameters.find(name) == _parameters.end())
      _parameters[name] = value;
    else
      {
        std::cerr << "ERROR: " << name << "  already defined !" << std::endl;
      }
  }

  //! set a model parameter
  void ProbabilisticModelParameters::set(std::string name, ProbabilisticModelParameterValuePtr value)
  {
    _parameters[name] = value;
  }


  //! Get a mandatory parameter
  ProbabilisticModelParameterValuePtr ProbabilisticModelParameters::getMandatoryParameterValue(const std::string & name) const {
    ProbabilisticModelParameterValuePtr null;
    std::map <std::string, ProbabilisticModelParameterValuePtr>::const_iterator it = _parameters.find(name) ;
    if(it == _parameters.end()) {
      std::cerr<< "ERROR: Mandatory parameter, " << name << ", was not defined !\n";
      return null;
    }
    return  it->second;
  }
  //! Get a optional parameter
  ProbabilisticModelParameterValuePtr ProbabilisticModelParameters::getOptionalParameterValue(const std::string & name) const {
    ProbabilisticModelParameterValuePtr null;
    std::map <std::string, ProbabilisticModelParameterValuePtr>::const_iterator it = _parameters.find(name) ;
    if(it == _parameters.end()) {
      return null;
    }
    return  it->second;
  }

  std::map <std::string, ProbabilisticModelParameterValuePtr>  ProbabilisticModelParameters::parameters() const {
    return _parameters;
  }

  void ProbabilisticModelParameterValue::setIsRoot(bool root) {
  }

  std::string ProbabilisticModelParameterValue::parameter_type () const {
    std::string type("ProbabilisticModelParameterValue");
    return type;
  }
  std::map<std::string,double> & ProbabilisticModelParameterValue::getDoubleMap()  {
    return p;
  }
  StringVector & ProbabilisticModelParameterValue::getStringVector() {
    return s;
  }
  DoubleVector & ProbabilisticModelParameterValue::getDoubleVector()  {
    return d;
  }
  IntVector & ProbabilisticModelParameterValue::getIntVector() {
    return i;
  }

  ProbabilisticModelParameters & ProbabilisticModelParameterValue::getParameters()  {
    return _parameters;
  }

  std::string ProbabilisticModelParameterValue::getString() const {
    return _str;
  }
  std::map<std::string,std::string> &  ProbabilisticModelParameterValue::getStringMap()  {
    return str_map;
  }
  int ProbabilisticModelParameterValue::getInt() const {
    int i = 0;
    std::cerr << "WARNING: parameter not specified as a int !\n"  << std::endl;
    return i;
  }
  double ProbabilisticModelParameterValue::getDouble()  const {
    double d = 0.0;
    std::cerr << "WARNING: parameter not specified as a double !\n"  << std::endl;
    return d;
  }
  std::string ProbabilisticModelParameterValue::str() const {
    std::string s;
    std::cerr << "WARNING: not implemented " << parameter_type() << "::str()" << std::endl;
    return s;
  }

  void ProbabilisticModelParameterListValue::initialize(ProbabilisticModelParameters p) {
    _parameters = p;
    _root = false;
  }
  ProbabilisticModelParameters & ProbabilisticModelParameterListValue::getParameters()  {
    return _parameters;
  }
  std::string ProbabilisticModelParameterListValue::parameter_type () const {
    std::string type("ProbabilisticModelParameterListValue");
    return type;
  }
  void ProbabilisticModelParameterListValue::setIsRoot(bool root) {
    _root = root;
  }

  std::string ProbabilisticModelParameterListValue::getString() const {
    std::stringstream out;
    std::map <std::string, ProbabilisticModelParameterValuePtr>::const_iterator it;
    std::map<std::string, ProbabilisticModelParameterValuePtr> p = _parameters.parameters();
    out << "[" <<std::endl;
    for(it = p.begin(); it != p.end(); it++)
      {
        out<< it->first << " = " << (it->second)->str() << std::endl;
      }
    out << "]";
    return out.str();
  }

  std::string ProbabilisticModelParameterListValue::str () const {
    std::stringstream out;
    std::map <std::string, ProbabilisticModelParameterValuePtr>::const_iterator it;
    std::map<std::string, ProbabilisticModelParameterValuePtr> p = _parameters.parameters();
    if(_root != true) {
      out << "[" <<std::endl;
      for(it = p.begin(); it != p.end(); it++)
        {
          out<< it->first << " = " << (it->second)->str() << std::endl;
        }
      out << "]" << std::endl;
    } else
      for(it = p.begin(); it != p.end(); it++)
        {
          out<< it->first << " = " << (it->second)->str() << std::endl;
        }

    return out.str();
  }


  void DoubleParameterValue::initialize(double d){
    _d = d;
  }

  std::string DoubleParameterValue::parameter_type () const {
    std::string type("DoubleParameterValue");
    return type;
  }

  double DoubleParameterValue::getDouble() const {
    return _d;
  }
  int DoubleParameterValue::getInt() const {
    return (int)_d;
  }

  std::string DoubleParameterValue::str() const {
    std::stringstream out;
    out << _d ;
    return out.str();
  }

  std::string IntParameterValue::parameter_type () const {
    std::string type("IntParameterValue");
    return type;
  }


  void IntParameterValue::initialize(int v) {
    _v = v;
  }

  int IntParameterValue::getInt() const {
    return _v;
  }
  double IntParameterValue::getDouble() const {
    return (double) _v;
  }

  std::string IntParameterValue::str() const {
    std::stringstream out;
    out << _v;
    return out.str();
  }


  std::string StringParameterValue::parameter_type () const {
    std::string type("StringParameterValue");
    return type;
  }
  void StringParameterValue::initialize(std::string v) {
    _v = v;
  }

  std::string StringParameterValue::getString() const {
    return _v;
  }

  std::string StringParameterValue::str() const {
    std::stringstream out;
    out << "\"" << _v << "\"" ;
    return out.str();
  }

  std::string IntVectorParameterValue::parameter_type () const {
    std::string type("IntVectorParameterValue");
    return type;
  }
  void IntVectorParameterValue::initialize(IntVector v) {
    _v = v;
  }

  IntVector & IntVectorParameterValue::getIntVector()  {
    return _v;
  }
  std::string IntVectorParameterValue::str() const {
    std::stringstream out;
    out << "(";
    if(_v.size() > 0)
      out << _v[0] ;
    for(int i = 1; i <(int) _v.size(); i++)
      out << "," << _v[i];
    out << ")";
    return out.str();
  }

  void DoubleMapParameterValue::initialize(const std::map<std::string, double > & values) {
    _v = values;
  }

  std::string DoubleMapParameterValue::parameter_type () const {
    std::string type("DoubleMapParameterValue");
    return type;
  }

  std::map<std::string, double> & DoubleMapParameterValue::getDoubleMap()   {
    return _v;
  }

  std::string DoubleMapParameterValue::str() const {
    std::map<std::string, double>::const_iterator it;
    std::stringstream out;
    out << "(";
    for(it = _v.begin(); it != _v.end(); it++)
      {
        std::vector <std::string> splited;
        boost::regex separator("\\|");
        split_regex(it->first, splited, separator);
        if(splited.size() == 1) {
          splited.push_back("");
        }
        std::string context (splited[1]);
        std::string symbol ( splited[0]);
        if(context.size() > 0)
          out<< "\"" << symbol << "\"|\"" << context << "\": " << (it->second) << ";" << std::endl;
        else
          out<< "\"" << symbol << "\": " << (it->second) << ";" << std::endl;
      }
    out << ")";
    return out.str();
  }

  void DoubleVectorParameterValue::initialize(DoubleVector v) {
    _v = v;
  }

  DoubleVector & DoubleVectorParameterValue::getDoubleVector()  {
    return _v;
  }
  std::string DoubleVectorParameterValue::parameter_type () const {
    std::string type("DoubleVectorParameterValue");
    return type;
  }

  std::string DoubleVectorParameterValue::str() const {
    std::stringstream out;
    out << "(";
    if(_v.size() > 0)
      out << _v[0] ;
    for(int i = 1; i < (int)_v.size(); i++)
      out << ", " << _v[i];
    out << ")";
    return out.str();
  }


  void StringMapParameterValue::initialize(std::map<std::string,std::string> m)
  {
    _str_map = m;
  }

  StringMap & StringMapParameterValue::getStringMap() {
    return  _str_map;
  }
  std::string StringMapParameterValue::parameter_type () const {
    std::string type("StringMapParameterValue");
    return type;
  }
  std::string StringMapParameterValue::str() const {
    std::map<std::string, std::string>::const_iterator it;
    std::stringstream out;
    out << "(";
    for(it = _str_map.begin(); it != _str_map.end(); it++)
      {
        out << "\"" <<it->first << "\": \"" <<  it->second << "\";" << std::endl;;
      }
    out << ")";
    return out.str();
  }



  StringVector &  StringVectorParameterValue::getStringVector() {
    return  _v;
  }
  void StringVectorParameterValue::initialize(StringVector v) {
    _v = v;
  }
  std::string StringVectorParameterValue::parameter_type () const {
    std::string type("StringVectorParameterValue");
    return type;
  }

  std::string StringVectorParameterValue::str() const {
    std::stringstream out;
    out << "(";
    if(_v.size() > 0)
      out << "\""<< _v[0] << "\"";
    for(int i = 1; i < (int)_v.size(); i++)
      out << ", \"" << _v[i] << "\"";
    out << ")";
    return out.str();
  }
}
