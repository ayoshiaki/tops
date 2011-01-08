/*
 *       FactorableModelPrefixSumArray.cpp
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

#include "FactorableModelPrefixSumArray.hpp"

namespace tops 
{

    
  //! Initialize the prefix sum array
  void FactorableModelPrefixSumArray::initialize(const Sequence & s) {
    if(_last == s)
      return;
    _last =s ;
    _alpha.resize(s.size() + 1);
    _precision.resize(s.size() + 1);
    _alpha[0] = 0;
    for(int i = 0 ; i < (int) s.size() ; i++) {
      double prob = _model->evaluate(s, i, i); 
      if(close(prob, -HUGE, 1e-1)) 
	{
	  _precision[i+1] = _precision[i]+1;
	  _alpha[i+1] = _model->evaluate(s,i,i);
	}
      else 
	{
	  _alpha[i+1] = _alpha[i] +   prob;
	  _precision[i+1] = _precision[i];
	}
    }
#if 0
    for(int i = 0; i < s.size(); i++)
      std::cerr << " "  << i << " " << m->alphabet()->getSymbol(s[i])->name() << " " << _alpha[i] << " " << _precision[i] << std::endl;
#endif 

  }
  
  //! compute the prefix sum array
  double FactorableModelPrefixSumArray::compute(int begin, int end ) const {
    if((begin >= 0) && ((end + 1) < (int) _alpha.size())) 
      {
	if((_precision[end+1] - _precision[begin]) > 0)
	  return -HUGE;
	return _alpha[end+1] - _alpha[begin];
      }
    else 
      return -HUGE;
  }
  

}
