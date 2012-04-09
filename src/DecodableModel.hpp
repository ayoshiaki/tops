/*
 *       DecodableModel.hpp
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

#ifndef DECODABLE_MODEL_HPP
#define DECODABLE_MODEL_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModel.hpp"
#include "Sequence.hpp"
#include "util.hpp"
#include "SparseMatrix.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {
  //! Interface defining probabilistic model with the viterbi, forward and backward algorithm
  class DLLEXPORT DecodableModel : public ProbabilisticModel  {
  public:
    DecodableModel(){};
    virtual ~DecodableModel(){};

    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;

    virtual Sequence & choose(Sequence & h, int size) const;

    virtual Sequence & choose(Sequence & h, Sequence & path,  int size) const;

    virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const;


    //! Forward algorithm
    virtual double forward(const Sequence & s, Matrix &alpha) const = 0;

    //! Backward algorithm
    virtual double backward(const Sequence & s, Matrix &beta) const = 0;

    //! Viterbi algorithm
    virtual double viterbi (const Sequence &s, Sequence &path, Matrix & gamma) const = 0;

      //! Choose a path given a sequence
      virtual void choosePath(const Sequence &s, Sequence &path);

    //! Posterior Probabilities: P(yi=k|x)
    virtual void posteriorProbabilities (const Sequence &s, Matrix & probabilities) const;
    virtual void posteriorProbabilities (const Sequence &s, SparseMatrixPtr probabilities) const;
      

    //! Posterior Decoding: ^yi = argmax_k P(yi=k|x)
    virtual void posteriorDecoding (const Sequence &s, Sequence &path, Matrix & probabilities) const;

    //! Choose the observation given a state
    /*!\param h is the history */
    virtual Sequence &  chooseObservation ( Sequence & h,int i, int state ) const = 0;

    //! Choose a state
    virtual int chooseState(int state ) const = 0;

    //! Choose the initial state
    virtual int chooseFirstState() const = 0;

    //! Get state name
    virtual std::string getStateName(int state) const = 0;

    //! Get the state names
    virtual AlphabetPtr getStateNames() const = 0;

    virtual DecodableModel* decodable() {
      return this;
    }
  };
  typedef boost::shared_ptr<DecodableModel>  DecodableModelPtr;
}

#endif
