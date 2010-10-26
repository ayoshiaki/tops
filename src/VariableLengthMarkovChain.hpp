#ifndef VLMC_MARKOV_CHAIN_HPP
#define VLMC_MARKOV_CHAIN_HPP

#include "ProbabilisticModel.hpp"
#include "Sequence.hpp"
#include "FactorableModel.hpp"
#include "ContextTree.hpp"


#include <boost/shared_ptr.hpp>


namespace tops{


  //! This class is a Variable Length Markov Chain
  class VariableLengthMarkovChain : public FactorableModel {
  public:
    VariableLengthMarkovChain(){
    };
    
    VariableLengthMarkovChain(ContextTreePtr tree) {
      _tree = tree;
    }
    virtual ~VariableLengthMarkovChain() {};

    //! Set the context tree of the Markov chain
    virtual void setTree(ContextTreePtr tree) {
      _tree = tree;
    }
    //! Returns the context tree
    virtual ContextTreePtr getTree(){
      return _tree;
    }

    //! Evaluate the position i of the sequence s.
    virtual double evaluatePosition(const Sequence & s, int i) const;

    //! Choose the position i of the sequence s given the subsequence before the position i.
    virtual double choosePosition(const Sequence & s, int i) const ;

    //! Returns a string representation of the model
    virtual std::string str() const ;

    //! Returns the name of the model
    virtual std::string model_name () const {
      return "VariableLengthMarkovChain";
    }

    //! Returns a factory for this Markov chain
    virtual ProbabilisticModelCreatorPtr getFactory() const ;

    //! Returns the number of parameters of the model
    virtual int size() const;

    //! Initialize the Markov chain given the parameters
    virtual void initialize(const ProbabilisticModelParameters & p) ;

    //! Returns the parameters of the model
    virtual ProbabilisticModelParameters parameters() const;
  private:
    ContextTreePtr _tree;

    void printDistribution(ContextTreePtr tree, ContextTreeNodePtr node, std::stringstream & out, AlphabetPtr alphabet) const ;


  };
  typedef boost::shared_ptr<VariableLengthMarkovChain> VariableLengthMarkovChainPtr;
}

#endif
