/*
 *       FixedSequenceAtPosition.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
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

#include "FixedSequenceAtPosition.hpp"
#include "Symbol.hpp"
namespace tops {

  void FixedSequenceAtPosition::initialize(int position, Sequence sequence, MultinomialDistributionPtr distr){
    _position = position;
    _sequence =sequence;
    _probabilities = distr;
  }
  double FixedSequenceAtPosition::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const{
    double result = ProbabilisticModelDecorator::evaluate(s, begin, end);
    int j;
    for(j = 0; (j < (int)_sequence.size()) && ((_position  + j) < (int)s.size()); j++)
      if(_sequence[j] != s[_position + j] )
	break;
    if(j != (int)_sequence.size())
      result += _probabilities->log_probability_of(1);
    else 
      result += _probabilities ->log_probability_of(0);
    return result;
  }
  void FixedSequenceAtPosition::addSequence(Sequence &h) const {
    if ((int)_probabilities->choose()  == 1)
      return;
    for(int i = _position; ((i-_position) < (int)_sequence.size()) && (i < (int)h.size()); i++)
      h[i] = _sequence[i-_position];
  }

  void FixedSequenceAtPosition::initializeMatchedPositions(const Sequence &s)  {
    _matchSeq.resize(s.size());
    for(int i = 0; i < (int)s.size(); i++)
      {
	int j;
	for(j = 0; (j < (int)_sequence.size()) && ((_position  + j) < (int)s.size()); j++)
	  if(_sequence[j] != s[_position + j] )
	    break;
	if(j == (int)_sequence.size())
	  _matchSeq[i] = true;
	else
	  _matchSeq[i] = false;
      }
  }

  Sequence & FixedSequenceAtPosition::choose(Sequence & h, int size) const {
    ProbabilisticModelDecorator::choose(h, size);
    addSequence(h);
    return h;
  }


  Sequence & FixedSequenceAtPosition::choose(Sequence &h, int initial_phase, int size) const{
    ProbabilisticModelDecorator::choose(h, initial_phase, size);
    addSequence(h);
    return h;
  }

  Sequence & FixedSequenceAtPosition::choose(Sequence & h, Sequence & path,  int size) const{
    ProbabilisticModelDecorator::choose(h, path, size);
    addSequence(h);
    return h;
  }

  Sequence & FixedSequenceAtPosition::choose(Sequence & h, Sequence & path,  int i, int size) const{
    ProbabilisticModelDecorator::choose(h, path, i, size);
    addSequence(h);
    return h;
  }

  Sequence & FixedSequenceAtPosition::chooseWithHistory(Sequence & h, int i,  int size) const{
    ProbabilisticModelDecorator::chooseWithHistory(h, i, size);
    addSequence(h);
    return h;
  }

  Sequence & FixedSequenceAtPosition::chooseWithHistory(Sequence & h, int i,  int phase, int size) const{
    ProbabilisticModelDecorator::chooseWithHistory(h, i, phase, size);
    addSequence(h);
    return h;
  }
  double FixedSequenceAtPosition::prefix_sum_array_compute(int begin , int end){
    double r = ProbabilisticModelDecorator::prefix_sum_array_compute(begin, end);
    if(_matchSeq[begin])
      r += _probabilities->log_probability_of(1);
    else 
      r+= _probabilities->log_probability_of(0);
    return r;
  }
  double FixedSequenceAtPosition::prefix_sum_array_compute(int begin , int end, int phase){
    double r = ProbabilisticModelDecorator::prefix_sum_array_compute(begin, end, phase);
    if(_matchSeq[begin])
      r += _probabilities->log_probability_of(1);
    else 
      r+= _probabilities->log_probability_of(0);
    return r;
  }

  bool FixedSequenceAtPosition::initialize_prefix_sum_array(const Sequence & s, int phase){
    if(ProbabilisticModelDecorator::initialize_prefix_sum_array(s, phase))
      {
	return true;
      }
    initializeMatchedPositions(s);
    return true;
  }

  bool FixedSequenceAtPosition::initialize_prefix_sum_array(const Sequence & s) {
    if(ProbabilisticModelDecorator::initialize_prefix_sum_array(s)){
      return true;
    }

    initializeMatchedPositions(s);
    return true;
  }
  std::string FixedSequenceAtPosition::model_name () const {
    return "FixedSequenceAtPosition";
  }

  std::string FixedSequenceAtPosition::str() const{
    std::stringstream out;
    out << "model_name =\"" << model_name() << "\"" << std::endl;
    out << "position = " << _position << std::endl;
    out << "sequences = ";
    if(_sequence.size() >= 1) {
      out << "\"" ;
      out << subModel()->alphabet()->getSymbol(_sequence[0])->name();
      for( int i = 1; i < (int)_sequence.size(); i++){
	out << " " ;
	out << subModel()->alphabet()->getSymbol(_sequence[i])->name();
      }
      out << "\"" << std::endl;
    }
    out << "probability = ";
    out << exp( _probabilities->log_probability_of(1));
    out << std::endl;

    std::string modelname = ProbabilisticModelDecorator::subModelName();
    if(modelname.length() > 0) 
      out << "model = " << modelname << std::endl ;
    else
      out << "model = [" << subModel()->str() << "]" << std::endl ;
    return out.str();
  }
}
