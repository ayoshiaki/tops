/*
 *       InhomogeneousMarkovChain.hpp
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

#ifndef WEIGHT_ARRAY_MODEL_HPP
#define WEIGHT_ARRAY_MODEL_HPP

#include "ContextTree.hpp"
#include "ProbabilisticModel.hpp"
#include "InhomogeneousFactorableModel.hpp"
#include "InhomogeneousMarkovChainCreator.hpp"
#include <boost/shared_ptr.hpp>

namespace tops {

  //! Inhomogeneous Markov chain
  class InhomogeneousMarkovChain : public InhomogeneousFactorableModel
  {
  private:
    std::vector <ContextTreePtr> _context_trees;
    bool _phased;


    void printDistribution(ContextTreePtr tree, ContextTreeNodePtr node, std::stringstream & out, AlphabetPtr alphabet) const;
  public:
    InhomogeneousMarkovChain(){
      _phased = false;
    }

    virtual void setPositionSpecificDistribution(std::vector<ContextTreePtr> position_specific_context_trees) {
      _context_trees = position_specific_context_trees;
    }

    //! Evaluate the position i of the sequence s.
    virtual double evaluatePosition(const Sequence & s, int i, int t) const;
    
    //! Choose the position i of the sequence s given the subsequence before the position i.
    virtual int choosePosition(const Sequence & s, int i, int t ) const;

    virtual int maximumTimeValue() const {
      return _context_trees.size()-1;
    }

    virtual bool isPhased() const{ 
      return _phased;
    }
    virtual void phased(bool phased) {
      _phased  = phased;
    }
    virtual std::string model_name() const {
      return "InhomogeneousMarkovChain";
    }
    virtual ProbabilisticModelCreatorPtr getFactory () const {
      return InhomogeneousMarkovChainCreatorPtr(new InhomogeneousMarkovChainCreator());
    }
    virtual InhomogeneousFactorableModel * inhomogeneous()  {
      return this;
    }
    virtual std::string str() const ;
    virtual int size() const ;

    virtual void removeSequenceFromModel(const Sequence & s,  int phase) ;

    virtual void initialize(const ProbabilisticModelParameters & p)  ;
    virtual ProbabilisticModelParameters parameters() const;
  };
  
  typedef boost::shared_ptr <InhomogeneousMarkovChain> InhomogeneousMarkovChainPtr;

}


#endif
