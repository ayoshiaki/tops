/*
 *       DecodableModel.cpp
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

#include "DecodableModel.hpp"

namespace tops {


  double DecodableModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
  {
    Matrix alpha;
    Sequence s2;
    s2.resize(end - begin + 1);
    for (unsigned int i = begin; i <= end; i++)
      s2[i-begin] = s[i];
    return forward(s2, alpha);
  }

  Sequence & DecodableModel::choose(Sequence & h, int size) const
  {
    Sequence p;
    return choose(h,p,0,size);
  }


  Sequence & DecodableModel::choose(Sequence & h, Sequence & path,   int size) const{
    return choose(h, path, 0, size);
  }

    void DecodableModel::choosePath(const Sequence &s, Sequence &path){
        std::cerr << model_name() <<  ":  not implemented choosePath" << std::endl;
    }

  Sequence & DecodableModel::choose(Sequence & h, Sequence & path,  int i, int size) const
  {
    assert(path.size() == h.size());
    int state = chooseFirstState();
    if (((i - 1) < (int) path.size()) && (i - 1) >= 0)
      state = path[i - 1];
    int l = 0;
    while ((int) h.size() < (i + size)) {
      chooseObservation(h, i + l, state);
      for (int k = i + l; k < (int) h.size(); k++, l++) {
        if (k < (int) path.size())
          path[k] = state;
        else
          path.push_back(state);
      }
      state = chooseState(state); // next state
    }
    h.resize(size);
    path.resize(size);
    return h;
  }

  //! Posterior Probabilities: P(yi=k|x)
  void DecodableModel::posteriorProbabilities (const Sequence &sequence, SparseMatrixPtr probabilities) const{
    return;
  }
    
  void DecodableModel::posteriorProbabilities (const Sequence &sequence, Matrix & probabilities) const
  {
    int nstates = (int)getStateNames()->size();
    int size = sequence.size();
    Matrix p (nstates, size);

    Matrix alpha; // forward
    Matrix beta;  // backward

    double full = forward(sequence, alpha);
    backward(sequence, beta);

    for(int k = 0; k < nstates; k++)
      for(int i = 0; i < size; i++)
        p(k, i) = alpha(k, i) + beta(k, i) - full;

    probabilities = p;
  }


  //! Posterior Decoding: ^yi = argmax_k P(yi=k|x)
  void DecodableModel::posteriorDecoding (const Sequence &sequence, Sequence &path, Matrix & probabilities) const{
    int nstates = (int)getStateNames()->size();
    int size = sequence.size();

    posteriorProbabilities(sequence, probabilities);

    path.resize(size);

    double max;
    for(int i = 0; i < size; i++)
    {
      max = probabilities(0, i);
      path[i] = 0;
      for(int k = 1; k < nstates; k++)
      {
        if(probabilities(k, i) > max)
        {
          max = probabilities(k, i);
          path[i] = k;
        }
      }
    }
  }

}
