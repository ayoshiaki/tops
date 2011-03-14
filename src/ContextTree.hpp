/*
 *       ContextTree.hpp
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

#ifndef CONTEXT_TREE_HPP
#define CONTEXT_TREE_HPP


#include "MultinomialDistribution.hpp"
#include "ProbabilisticModelParameter.hpp"
#include <boost/shared_ptr.hpp>
#include <set>
namespace tops {
  class ContextTreeNode;
  typedef boost::shared_ptr <ContextTreeNode> ContextTreeNodePtr;
  typedef std::vector< boost::shared_ptr<tops::ContextTreeNode> > ContextTreeNodeVector;
    class ContextTree;
   typedef boost::shared_ptr<ContextTree> ContextTreePtr;
  //! This is a context tree node
  class ContextTreeNode {
  private:
    MultinomialDistributionPtr _distribution;
    ContextTreeNodeVector _child;
    int _alphabet_size;
    int _symbol;
    bool _leaf;
    std::vector<double> _counter;
    int _id;
    int _parent_id;
  public:
    ~ContextTreeNode(){ }

    //! \param alphabet is the alphabet to be used
    ContextTreeNode(int alphabet_size);

    //! Default constructor
    ContextTreeNode() ;

    //! Add a count for the symbol s
    void addCount (int s);

    //! Add v to the  counter for the symbol s
    void setCount (int s, double v);

    //! get the counter
    std::vector<double> & getCounter ();

    //! Set the alphabet size
    int alphabet_size();


    //! Set the parent id
    void setParent(int parent) ;

    //! Get the parent id;
    int getParent() ;

    int id ();
    //! Set the id of the node
    void setId(int id);

    //! set the child for a given symbol
    void setChild(ContextTreeNodePtr child, int symbol);

    //! get the symbol of the node
    int symbol();

    //! get the symbol of the node
    void setSymbol(int symbol);


    //! set the distribution of this context
    void setDistribution(MultinomialDistributionPtr distribution);

    //! get the child of a symbol
    ContextTreeNodePtr getChild(int symbol);

    //! get the distribution of this context
    MultinomialDistributionPtr getDistribution();

    //! deletes the children of this contex
      void deleteChildren() ;

    //! Returns the children
    ContextTreeNodeVector getChildren() ;

    //! returns true if this context node is a leaf
    bool isLeaf();


    std::string str() const ;


  };


  //! This class represents a context tree
  class ContextTree {
  public:
    ~ContextTree() {
    }
    ContextTree(){}
    ContextTree(AlphabetPtr alphabet);

    ContextTreeNodeVector & all_context() {
      return _all_context;
    }

    //! return the root of the tree
    ContextTreeNodePtr getRoot() const ;


    //! Create new context
    ContextTreeNodePtr createContext() ;


    ContextTreeNodePtr getContext (int id) ;

    //! get the context for the sequence s[i-1], s[i-2], s[i-3]...
    ContextTreeNodePtr getContext(const Sequence & s, int i);

    std::set <int> getLevelOneNodes();

    void removeContextNotUsed();

    void normalize();

    //! normalize using a tree to get the a priori probabilities
      void normalize(ProbabilisticModelPtr old, double pseudocount);

    std::string str() const;

    void initializeCounter(const SequenceEntryList & sequences, int order);

    void initializeCounter(const SequenceEntryList & sequences, int order, double pseudocounts);

    //! Prune similar subtrees
    void pruneTree(double delta) ;


    //! Prune subtrees with small counters
    void pruneTreeSmallSampleSize(int small) ;

    //! Initialize context tree using the Rissanen algorithm
    void initializeContextTreeRissanen(const SequenceEntryList & sequences);

    DoubleMapParameterValuePtr getParameterValue () const;

    int getNumberOfNodes() const
    {
      return _all_context.size();
    }

  private:
    void printTree(ContextTreeNodePtr node, std::stringstream & out) const;
    void buildParameters(ContextTreeNodePtr node, std::map<std::string, double> & parameters) const;
    ContextTreeNodeVector _all_context;
    AlphabetPtr _alphabet;

  };
  typedef boost::shared_ptr<ContextTree> ContextTreePtr;


}

#endif
