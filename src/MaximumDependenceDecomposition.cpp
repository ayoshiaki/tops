    /*
 *       MaximumDependenceDecomposition.cpp
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

#include "MaximumDependenceDecomposition.hpp"
#include "Alphabet.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "Symbol.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "ProbabilisticModelCreatorClient.hpp"

namespace tops{

  void restore_submodel(std::string & model_name, std::map<std::string, ProbabilisticModelPtr> &_models,  const ProbabilisticModelParameters & parameters);

    //! Calculates the sequence likelihood given this model
  double MaximumDependenceDecomposition::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
  {
    double result = 0.0;
    std::stringstream node;
    std::vector <int> rest;
    for(int i = 0; i < _max_size; i++){
      rest.push_back(0);
    }
    node << "x";
    while(1) {
      std::string node_id = node.str();
      int pos = _positions.find(node_id)->second;
      result += (_submodels.find(node_id)->second)->inhomogeneous()->evaluatePosition(s, pos, pos);
      rest[pos] = 1;
      std::string last_node = node_id;
      if(s[pos] == _consensus.find(node_id)->second) {
	node << "l";
      } else {
	node << "r";
      }
      // if we reach the end of the tree, calculate the probabilities using the last model
      if(_positions.find(node.str()) == _positions.end()) {
	for(int r = 0; r < _max_size; r++) 
	  if(rest[r] == 0) {
	    result += _submodels.find(last_node)->second->inhomogeneous()->evaluatePosition(s, r, r);
	  }
	break;
      }
    }
    return result;
  }

  Sequence & MaximumDependenceDecomposition::choose(Sequence & s, int size) const {
    std::stringstream node;
    std::vector <int> rest;
    for(int i = 0; i < _max_size; i++){
      rest.push_back(0);
    }
    node << "x";
    while(1) {
      std::string node_id = node.str();
      int pos = _positions.find(node_id)->second;
      s[pos] = (_submodels.find(node_id)->second)->inhomogeneous()->choosePosition(s, pos, pos);
      rest[pos] = 1;
      std::string last_node = node_id;
      if(s[pos] == _consensus.find(node_id)->second) {
	node << "l";
      } else {
	node << "r";
      }
      // if we reach the end of the tree, complete the sequence using the last model
      if(_positions.find(node.str()) == _positions.end()) {
	for(int r = 0; r < _max_size; r++) 
	  if(rest[r] == 0) {
	    s[r] = _submodels.find(last_node)->second->inhomogeneous()->choosePosition(s, r, r);
	  }
	break;
      }
    }
    return s;
  }


  void MaximumDependenceDecomposition::initialize(const ProbabilisticModelParameters & p) {
    ProbabilisticModelParameterValuePtr maxsizepar = p.getMandatoryParameterValue("max_length");
    ProbabilisticModelParameterValuePtr consensuspar = p.getMandatoryParameterValue("consensus");
    ProbabilisticModelParameterValuePtr positionspar = p.getMandatoryParameterValue("positions");
    _max_size = maxsizepar->getInt();
    std::map<std::string,double> aux = consensuspar->getDoubleMap();
    std::map<std::string,double>::const_iterator it; 
    for (it = aux.begin(); it != aux.end(); it++) {
      _consensus [it->first] = (int)it->second;
    }
    aux = positionspar->getDoubleMap();
    for (it = aux.begin(); it != aux.end(); it++) {
      _positions [it->first] = (int)it->second;
    }
    for (it = aux.begin(); it != aux.end(); it++) {
      std::string node = it->first;
      restore_submodel(node, _submodels, p);
    }
    _parameters = p;
  }

 std::string MaximumDependenceDecomposition::str() const{
    std::stringstream s;
    std::map <std::string, ProbabilisticModelParameterValuePtr> ::const_iterator it;
    std::map <std::string, ProbabilisticModelParameterValuePtr> p = _parameters.parameters();
    for(it = p.begin(); it != p.end(); it++)
      {
	s << it->first << " = " << (it->second)->str() << std::endl;
      }
    return s.str();
  }

  ProbabilisticModelParameters MaximumDependenceDecomposition::parameters() const{
    return _parameters;
  }
}

