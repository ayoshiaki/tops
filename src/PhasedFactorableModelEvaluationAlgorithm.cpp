/*
 *       PhasedFactorableModelEvaluationAlgorithm.cpp
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

#include "PhasedFactorableModelEvaluationAlgorithm.hpp"
#include "ThreePeriodicMarkovChain.hpp"

namespace tops {

  double  PhasedFactorableModelEvaluationAlgorithm::compute(int begin, int end, int phase) const
  {
    int nphase = _alpha.size();
    int p = mod(begin, nphase);
    int k = 0;
    while((phase >= 0) &&  (mod(p + k, nphase) !=  phase))
      k++;
    p = k;
#if 0
    std::cerr << p << " " << begin <<  " " << end << " " << phase << " " << _alpha[p][begin] << " " << _alpha[p][end+1] << " " << _precision[p][end+1] - _precision[p][begin] <<  std::endl;
#endif
    if((end  + 1) >= _precision[p].size())
      end = _precision[p].size() - 2;
    double t = _precision[p][end+1] - _precision[p][begin];
    if(t > 0)
      return -HUGE;

    return _alpha[p][end+1] - _alpha[p][begin];
  }

  double PhasedFactorableModelEvaluationAlgorithm::compute(int begin, int end) const
  {
    int nphase = _alpha.size();
    int p = mod(begin, nphase);
    int k = 0;
    while((_phase > 0) &&  (mod(p + k, nphase) !=  _phase))
      k++;
    p = k;
    double t = _precision[p][end+1] - _precision[p][begin];
    if(t > 0)
      return -HUGE;
    return _alpha[p][end+1] - _alpha[p][begin];
  }

  void PhasedFactorableModelEvaluationAlgorithm::initialize(const Sequence & s, const ProbabilisticModel *m)
  {
    _alpha.resize(1);
    for(int i = 0; i < (int)s.size(); i++)
      _alpha[0][i] += m->evaluate(s, i,i);
    _phase = 0;
  }


  void PhasedFactorableModelEvaluationAlgorithm::initialize(const Sequence & s, const ProbabilisticModel *m, int phase)
  {
    _phase = phase;
    int nphases = m->numberOfPhases();
    _alpha.resize(nphases);
    _precision.resize(nphases);
    Sequence history;
    for(int k = 0; k < nphases; k++)
      {
        _alpha[k].resize(s.size() +1);
        _precision[k].resize(s.size()+1);
      }
    for(int k = 0; k < nphases; k++)
      {
        _alpha[k][0] = 0;
        _precision[k][0] = 0;
        history.resize(0);
        for(int i = 0; i < (int)s.size(); i++)
          {
            double prob = m->log_probability_of (history, s, i, k);
            if(close(prob, -HUGE, 1))
              {
                _precision[k][i+1] = _precision[k][i]+ 1;
                history.resize(0);
                _alpha[k][i+1] = m->log_probability_of(history,s,i,k);

              }
            else
              {
                _precision[k][i+1] = _precision[k][i];
                _alpha[k][i+1] = _alpha[k][i] + prob;
                history.push_back(s[i]);
              }
          }
      }
#if 0
    for(int k = 0; k < nphases; k++){
      std::cerr << k << std::endl;
      for(int i = 0; i < s.size(); i++)
        std::cerr << " "  << i << " " << m->alphabet()->getSymbol(s[i])->name() << " " << _alpha[k][i] << " " << _precision[k][i] << std::endl;
    }
#endif
  }

}

