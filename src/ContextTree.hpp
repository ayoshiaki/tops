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
