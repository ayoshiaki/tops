#ifndef PROBABILISTIC_MODEL_HPP
#define PROBABILISTIC_MODEL_HPP

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
  class DecodableModel;
  class FactorableModel;
  class InhomogeneousFactorableModel;
  class ProbabilisticModelCreator;
  typedef boost::shared_ptr<ProbabilisticModelCreator> ProbabilisticModelCreatorPtr;

  //! This is an abstract class representing a generative probabilistic model.
  class ProbabilisticModel
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


    virtual double prefix_sum_array_compute(int begin , int end) {
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
      not_implemented("inhomogeneous()");
      return NULL;
    }
    virtual DecodableModel* decodable() {
      not_implemented("decodable()");
      return NULL;
    }

    virtual FactorableModel* factorable() {
      not_implemented("factorable()");
      return NULL;
    }
  };

  typedef boost::shared_ptr<ProbabilisticModel> ProbabilisticModelPtr;
};
#endif
