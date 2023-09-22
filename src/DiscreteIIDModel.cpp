/*
 *       DiscreteIIDModel.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bonadio <ibonadio@ime.usp.br>
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

#include "DiscreteIIDModel.hpp"
#include "DiscreteIIDModelCreator.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>

namespace tops {

  DiscreteIIDModel:: DiscreteIIDModel(const DoubleVector & probabilities)
  {
    _log_probabilities.resize(probabilities.size());
    _size = probabilities.size() - 1; // model size is the number of free parameters
    double  sum = 0;
    for(unsigned int i = 0; i < probabilities.size(); i++)
      sum += probabilities[i];
    for(int i = 0; (i < _size); i++){
      _log_probabilities[i] = -HUGE;
      if(!close(probabilities[i], 0.0, 1e-200))
	_log_probabilities[i] = log(probabilities[i]/sum);
    }
    _geometric_tail = false;
  }

  DiscreteIIDModel::DiscreteIIDModel(const Matrix & probabilities){
    int size1 = probabilities.size1();
    int size2 = probabilities.size2();
    _size = size1 * size2 - 1;
    _log_probabilities_matrix.resize(size1, size2);
    double sum = 0;
    for(int i = 0; i < size1; i++){
      for(int j = 0; j < size2; j++){
        if(close(probabilities(i,j), 0.0, 1e-10))
          continue;
        sum += probabilities(i,j);
      }
    }
    for(int i = 0; i < size1; i++){
      for(int j = 0; j < size2; j++){
        if(close(probabilities(i,j), 0.0, 1e-10))
          _log_probabilities_matrix(i,j) = -HUGE;
        else
          _log_probabilities_matrix(i,j) = log(probabilities(i,j)/sum);
      }
      _geometric_tail = false;
    }
  }

  void DiscreteIIDModel::setProbabilities(const DoubleVector & probabilities) {
    _size = probabilities.size() - 1;
    _log_probabilities.resize(_size+1);
    double  sum = 0;
    for(unsigned int i = 0; i < probabilities.size(); i++)
      sum += probabilities[i];
    for(int i = 0; (i <= _size); i++)
      if(i >= (int)probabilities.size())
        _log_probabilities[i] = -HUGE;
      else if(close(probabilities[i], 0.0, 1e-10))
        _log_probabilities[i] = -HUGE;
      else
        _log_probabilities[i] = log(probabilities[i]/sum);
  }
 double DiscreteIIDModel::choosePosition(const Sequence & s, int i ) const {
   return choose();
 }

  double DiscreteIIDModel::evaluatePosition(const Sequence & s, unsigned int i) const  {
    return log_probability_of(s[i]);
  }

   double DiscreteIIDModel::log_probability_of(int s) const  {
     if ((s>=0) && (s < (int)(_log_probabilities.size() )))
        return _log_probabilities[s];
    return -HUGE;
   }

  double DiscreteIIDModel::log_probability_of_pair(int s1, int s2) const  {
     if ((s1>=0) && (s1 < (int)(_log_probabilities_matrix.size1())) && (s2>=0) && (s2 < (int)(_log_probabilities_matrix.size2())))
       return _log_probabilities_matrix(s1,s2);
      else {
        return -HUGE;
      }
   }

  double DiscreteIIDModel::log_probability_of(int s, double new_value) {
    _log_probabilities[s] = new_value;
    return log_probability_of(s);
  }

  double DiscreteIIDModel::log_probability_of_pair(int s1, int s2, double new_value) {
    _log_probabilities_matrix(s1,s2) = new_value;
    return log_probability_of_pair(s1,s2);
  }

  int DiscreteIIDModel::size() const
  {
    return _size;
  }

  double DiscreteIIDModel:: choose() const {
    double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
    int result;
    for(result = 0; result < (int)_log_probabilities.size(); result++)
      {
        random -= exp(_log_probabilities[result]);
        if(random <= 0)
          break;
      }
    if(result >= (int)_log_probabilities.size())
      return (double)(result-1);
    return (double)result;
  }

  void DiscreteIIDModel::choosePair(int* a, int* b) const {
    double random = ((double)rand())/ ((double) RAND_MAX + 1.0) ;
    int result1 = 0;
    int result2 = 0;
    for(result1 = 0; result1 < (int)_log_probabilities_matrix.size1(); result1++){
      for(result2 = 0; result2 < (int)_log_probabilities_matrix.size2(); result2++){
        random -= exp(_log_probabilities_matrix(result1,result2));
        if(random <= 0)
          break;
      }
      if(random <= 0)
        break;
    }
    if(result1 >= (int)_log_probabilities_matrix.size1() && result2 >= (int)_log_probabilities_matrix.size2()){
      *a = (result1 - 1);
      *b = (result2 - 1);
    }
    else{
      *a = result1;
      *b = result2;
    }
  }

  ProbabilisticModelCreatorPtr DiscreteIIDModel::getFactory () const{
    return DiscreteIIDModelCreatorPtr(new DiscreteIIDModelCreator());
  }

  std::string DiscreteIIDModel::str () const {
    std::stringstream out;
    AlphabetPtr alpha = alphabet();

    out << "model_name = \"DiscreteIIDModel\"\n" ;
    out << "probabilities = (";
    if(_log_probabilities.size() > 0){
      out << exp(_log_probabilities[0]);
      for(int k = 1; k < (int)_log_probabilities.size(); k++)
        {
          out << "," << exp(_log_probabilities[k]);
        }
      out << ")\n";
    }
    if(alpha != NULL)
      {
        out << alpha->str();
      }
    return out.str();
  }


  void DiscreteIIDModel::strMatrix () const {
    if(_log_probabilities_matrix.size1() > 0){
      for(int i = 0; i < (int)_log_probabilities_matrix.size1(); i++){
        for(int j = 0; j < (int)_log_probabilities_matrix.size2(); j++){
          if(j == (int)_log_probabilities_matrix.size2() -1)
            std::cout << exp(_log_probabilities_matrix(i,j)) << endl;
          else
            std::cout << exp(_log_probabilities_matrix(i,j)) << " ";
        }
      }
      cout << endl << endl;
    }
  }

  void DiscreteIIDModel::initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet)
  {
    std::map<std::string,double>::const_iterator it;
    DoubleVector probs;
    probs.resize(alphabet->size());
    for(int i = 0; i < (int)alphabet->size()  ; i++)
      probs[i] = 0.0;
    for(it = probabilities.begin(); it != probabilities.end(); it++)
    {
      int id = alphabet->getSymbol(it->first)->id();
      if(id < (int)probs.size())
	probs[id] = it->second;
    }
    _log_probabilities.resize(probs.size());
    double  sum = 0;
    for(unsigned int i = 0; i < probs.size(); i++)
    {
      sum += probs[i] ;
      if(close(probs[i], 0.0, 1e-100))
	_log_probabilities[i] = -HUGE;
      else
	_log_probabilities[i] = log(probs[i]);
    }
    if(close(sum, 1.0, 1e-100))
      _log_probabilities.push_back(-HUGE);
    else {
      _log_probabilities[probs.size()-1] = log(1.0 - sum);
      std::cerr << "WARNING: DiscreteIIDModel parameters must sum 1.0: sum is " << sum << std::endl;
    }
    setAlphabet(alphabet);
  }

  void DiscreteIIDModel::initialize(const ProbabilisticModelParameters & p) {
    ProbabilisticModelParameterValuePtr probs = p.getMandatoryParameterValue("probabilities");
    ProbabilisticModelParameterValuePtr alphabet = p.getOptionalParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr geometric_tail_ptr = p.getOptionalParameterValue("geometric_tail");
    DoubleVector distr = probs->getDoubleVector();
    setProbabilities(distr);
    if (alphabet != NULL)
      {
        AlphabetPtr alpha = AlphabetPtr(new Alphabet());
        alpha->initializeFromVector(alphabet->getStringVector());
        setAlphabet(alpha);
        if(geometric_tail_ptr != NULL) {
          _geometric_tail = (geometric_tail_ptr->getInt() == 1);
        }
      }
    _mean = 0.0;
    for (int i = 0 ; i < _log_probabilities.size(); i++){
      _mean += exp(_log_probabilities[i]) * i;
    }

  }

  ProbabilisticModelParameters DiscreteIIDModel::parameters () const {
    AlphabetPtr alpha = alphabet();
    ProbabilisticModelParameters par;
    par.add("model_name", StringParameterValuePtr(new StringParameterValue("DiscreteIIDModel")));
    DoubleVector probs;
    for(int k = 0; k < (int)_log_probabilities.size(); k++)
      {
        probs.push_back( exp(_log_probabilities[k]));
      }
    par.add("probabilities", DoubleVectorParameterValuePtr(new DoubleVectorParameterValue(probs)));
    if(alpha->size() > 0)
      par.add("alphabet", alpha->getParameterValue());
    if(_geometric_tail == 1)
      par.add("geometric_tail", IntParameterValuePtr(new IntParameterValue(1)));
    return par;
  }
}


