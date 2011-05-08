/*
 *       ProbabilisticModel.hpp
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

#ifndef PROBABILISTIC_MODEL_HPP
#define PROBABILISTIC_MODEL_HPP

#include "crossplatform.hpp"

#include <cmath>
#include <iostream>

#include <vector>
#include <cstdarg>
#include <boost/shared_ptr.hpp>

#include "util.hpp"
#include "Sequence.hpp"
#include "Alphabet.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "GHMMStates.hpp"

namespace tops {
  class DLLEXPORT DecodableModel;
  class DLLEXPORT PairHiddenMarkovModel;
  class DLLEXPORT FactorableModel;
  class DLLEXPORT InhomogeneousFactorableModel;
  class DLLEXPORT ProbabilisticModelCreator;
  typedef boost::shared_ptr<ProbabilisticModelCreator> ProbabilisticModelCreatorPtr;

  //! This is an abstract class representing a generative probabilistic model.
  class DLLEXPORT ProbabilisticModel
  {
  private:
    AlphabetPtr _alphabet;
    Sequence  _last;
    double _dummy;
  public:
    ProbabilisticModel () {
      _alphabet = AlphabetPtr(new Alphabet());
    }
    ProbabilisticModel( AlphabetPtr alpha) : _alphabet(alpha)   {    };
    virtual ~ProbabilisticModel () {}

    //! Calculates the sequence likelihood given this model
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const {
      not_implemented("evaluate()");
      return 0;
    }

    //! Calculates the probability of Pr(Si = s[i]  | s[i-1],...,s[0])
    virtual double evaluatePosition(const Sequence & s, unsigned int i) const {
      not_implemented("evaluatePosition()");
      return 0;
    }


    //! Calculates the probability of Pr_t(Si = s[i]  | s[i-1],...,s[0])
      virtual double evaluatePosition(const Sequence & s,  int i,  int t) const {
          evaluatePosition(s, i);
          return 0;
      }


    //! Choose a symbol
    virtual double choose() const {
      not_implemented("choose()");
      return 0;
    }
    virtual Sequence & choose(Sequence & h, int size) const {
      not_implemented("choose(Sequence &h, int size)");
      return h;
    }

    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const{
      choose(h, size);
      not_implemented("choose(Sequence &h, int initial_phase, int size)");
      return h;
    }

    virtual Sequence & choose(Sequence & h, Sequence & path,  int size) const
    {
      choose(h, size);
      //      not_implemented("choose(Sequence &h, Sequence & path, int size)");
      return h;
    }


    virtual Sequence & choose(Sequence & h, Sequence & path,  int i, int size) const
    {
      not_implemented("choose(Sequence &h, Sequence & path, int size)");
      return h;
    }

    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int size) const
    {
      choose(h,size);
      return h;
    }

    virtual Sequence & chooseWithHistory(Sequence & h, int i,  int phase, int size) const{
      chooseWithHistory(h,i, size);
      return h;
    }

    //! Returns the alphabet
    virtual AlphabetPtr alphabet() const
    {
      return _alphabet;
    }
    //! Setter for the Alphabet
    virtual void setAlphabet(AlphabetPtr alphabet)
    {
      _alphabet = alphabet;
    }
    //! returns the string representation of the model
    virtual std::string str() const {
      std::stringstream out;
      not_implemented("str()");
      return out.str();
    }

    //! returns the model name
    virtual std::string model_name() const{
      std::stringstream out;
      not_implemented("model_name()");
      return out.str();
    }

    //! Returns the number of parameters of the model
    virtual int size() const {
      not_implemented("size()");
      return 0;
    }

    virtual ProbabilisticModelParameters parameters() const {
      ProbabilisticModelParameters p;
      std::cerr << "ERROR: The " << model_name() << " does not implement parameters" << std::endl;
      exit(-1);
      return p;
    }

    virtual void initialize(const ProbabilisticModelParameters & p) {
      std::cerr << "ERROR: The " << model_name() << " does not implement initialize" << std::endl;
      exit(-1);
    }

    virtual void not_implemented (std::string method ) const {
      std::cerr << "ERROR: The " << model_name() << " does not implement " << method << std::endl;
    }
    //! Returns the log_probability_of the number s
    virtual double  log_probability_of(int s) const {
      not_implemented("log_probability_of(int s) const");
      return 0.0;
    }

    //! Set the probability value of the number s
    virtual double probability_of(int s, double new_value) {
      not_implemented("probability_of(int s, double new_value)");
      return _dummy;
    }
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end, int phase) const {
      //      not_implemented("evaluate(const Sequence & s int begin, int end, int phase)");
      return evaluate(s, begin, end);

    }

    virtual double prefix_sum_array_compute(int begin , int end) {
        std::cerr << "WARNING: " << model_name() << " does not implement prefix_sum_array " << std::endl;
      return evaluate(_last, begin, end);
    }

    virtual double prefix_sum_array_compute(int begin , int end, int phase) {
      return prefix_sum_array_compute(begin,end);
    }

    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase) {
      return initialize_prefix_sum_array(s);
    }
    virtual bool initialize_prefix_sum_array(const Sequence & s) {
      if(_last == s)
        return true;
      _last = s;
      return false;
    }
    virtual void removeSequenceFromModel(const Sequence & s, int phase) {
      not_implemented("removeSequenceFromModel()");
    }
    virtual AlphabetPtr getStateNames() const {
      AlphabetPtr n;
      not_implemented("getStateNames()");
      return n;
    }

    //! Train baum Welch
    virtual void trainBaumWelch (SequenceList & training_set, int maxiterations, double diff) {
      not_implemented("trainBaumWelch");
    }


    virtual InhomogeneousFactorableModel* inhomogeneous() {
        //      not_implemented("inhomogeneous()");
      return NULL;
    }
    virtual DecodableModel* decodable() {
        //      not_implemented("decodable()");
      return NULL;
    }

    virtual PairHiddenMarkovModel* pairDecodable() {
      not_implemented("pairDecodable()");
      return NULL;
    }

    virtual FactorableModel* factorable() {
        //      not_implemented("factorable()");
      return NULL;
    }
    virtual std::string print_graph () const {
      not_implemented("print_graph()");
      return "";
    }
  };

  typedef boost::shared_ptr<ProbabilisticModel> ProbabilisticModelPtr;
};
#endif
