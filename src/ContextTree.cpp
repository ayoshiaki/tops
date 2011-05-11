/*
 *       ContextTree.cpp
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

#include "ContextTree.hpp"
#include "util.hpp"
#include <vector>
#include "Symbol.hpp"

namespace tops
{
  void ContextTree::buildParameters(ContextTreeNodePtr node, std::map<std::string, double> & parameters) const
  {
    ContextTreeNodePtr current = node;
    AlphabetPtr alphabet = current->getDistribution()->alphabet();
    std::vector <std::string> aux;
    bool root=true;
    if(current != getRoot() )
      root = false;
    while(current != getRoot())
      {
        aux.push_back(alphabet->getSymbol(current->symbol())->name() );
        current = _all_context[current->getParent()];
      }
    for(int k = 0; k < (int)alphabet->size(); k++)
      {
        std::stringstream out;
        out << alphabet->getSymbol(k)->name() << "|" ;
        if(!root)
          {
            out << aux[aux.size() -1];
            for(int i = aux.size()-2; i >=0; i--)
              out << " " << aux[i] ;
          }
        parameters[out.str()] =  exp(node->getDistribution()->log_probability_of(k));
      }
    if(!node->isLeaf())
      for(int l = 0; l < node->alphabet_size(); l++)
        if(node->getChild(l) != NULL)
          buildParameters(node->getChild(l), parameters);

  }
  DoubleMapParameterValuePtr ContextTree::getParameterValue () const
  {
    std::map<std::string, double> probabilities;
    buildParameters(getRoot(), probabilities);
    DoubleMapParameterValuePtr v = DoubleMapParameterValuePtr(new DoubleMapParameterValue(probabilities));
    return v;
  }
  ContextTreeNode::ContextTreeNode(int alphabet_size)
  {
    _child.resize(alphabet_size);
    _counter.resize(alphabet_size);
    for(int i = 0; i < (int)_counter.size(); i++)
      _counter[i] = 0;
    _symbol = -1;
    _alphabet_size = alphabet_size;
    _leaf = true;
    _id = 0;
  }

  ContextTreeNode::ContextTreeNode() {
    _symbol = -1;
    _alphabet_size = 0;
    _leaf = true;
  }

  void ContextTreeNode::addCount (int s) {
    _counter[s] += 1.0;
  }

  void ContextTreeNode::addCount (int s, double weight) {
    _counter[s] += weight;
  }


  void ContextTreeNode::setCount (int s, double v) {
    _counter[s] = v;
  }

  std::vector<double> & ContextTreeNode::getCounter () {
    return _counter;
  }

  int ContextTreeNode::alphabet_size() {
    return _alphabet_size;
  }

  void ContextTreeNode::setParent(int parent)
  {
    _parent_id = parent;
  }
  int ContextTreeNode::getParent()
  {
    return _parent_id;
  }
  int ContextTreeNode::id () {
    return _id;
  }
  void ContextTreeNode::setId(int id)
  {
    _id = id;
  }

  void ContextTreeNode::setChild(ContextTreeNodePtr child, int symbol){
    assert((symbol >= 0) && (symbol < (int)_child.size()));
    _child[symbol] = child;
    child->setSymbol(symbol);
    child->setParent(id());
    _leaf = false;
  }

  int ContextTreeNode::symbol(){
    return _symbol;
  }

  void ContextTreeNode::setSymbol(int symbol){
    _symbol = symbol;
  }

  void ContextTreeNode::setDistribution(MultinomialDistributionPtr distribution){
    _distribution = distribution;
  }
  ContextTreeNodePtr ContextTreeNode::getChild(int symbol){
    if(!((symbol >= 0) && (symbol < (int)_child.size())))
      {
        std::cerr << "ERROR: ContextTree has reached an invalid node !" << std::endl;
        std::cerr << "Symbol id : " << symbol << std::endl;
        exit(-1);
      }
    return _child[symbol] ;
  }
  MultinomialDistributionPtr ContextTreeNode::getDistribution(){
    return _distribution;
  }
  void ContextTreeNode::deleteChildren() {
    ContextTreeNodePtr n;
    for(int m = 0; m < (int)_child.size(); m++)
      if(_child[m] != NULL)
        _child[m]->setParent(-1);
    _child.resize(0);
    _child.resize(_alphabet_size);
    _leaf = true;
  }
  std::vector <ContextTreeNodePtr> ContextTreeNode::getChildren() {
    return _child;
  }



  bool ContextTreeNode::isLeaf(){
    return _leaf;
  }

  std::string ContextTreeNode::str() const {
    std::stringstream out;
    return out.str();
  }



  void ContextTree::printTree(ContextTreeNodePtr node, std::stringstream & out) const
  {
    ContextTreeNodePtr current = node;
    while(current != getRoot())
      {
        out << current->symbol() << " ";
        current = _all_context[current->getParent()];
      }
    int sum = 0;
    out << ": ";
    for(int k = 0; k < (int)_alphabet->size(); k++)
      {
        sum += (node->getCounter())[k];
        out << (node->getCounter())[k] << " " ;
      }

    out << "("<< node->id() << ", "<<  sum << ") ";
    if(!node->isLeaf())
      out << "internal node" << std::endl;
    else
      out << "leaf node" << std::endl;
    if(!node->isLeaf())
      for(int l = 0; l < node->alphabet_size(); l++)
        if(node->getChild(l) != NULL)
          printTree(node->getChild(l), out);
  }




  ContextTree::ContextTree(AlphabetPtr alphabet){
    _alphabet = alphabet;
  }

  ContextTreeNodePtr ContextTree::getRoot() const {
    return _all_context[0];
  }


  ContextTreeNodePtr ContextTree::createContext() {
    ContextTreeNodePtr n = ContextTreeNodePtr(new ContextTreeNode(_alphabet->size()));
    n->setId(_all_context.size());
    if(n->id() == 0)
      {
        n->setParent(0);
      }
    _all_context.push_back(n);
    return n;
  }

  ContextTreeNodePtr ContextTree::getContext (int id)
  {
    return _all_context[id];
  }


  //! get the context for the sequence s[i-1], s[i-2], s[i-3]...
  ContextTreeNodePtr ContextTree::getContext(const Sequence & s, int i){
    ContextTreeNodePtr c = _all_context[0];
    ContextTreeNodePtr p;
    int j;
    for(j = i-1; j >=0; j--){
      if(c->isLeaf())
        break;
      p = c;
      c = c->getChild(s[j]);
      if(c == NULL)
        {
          c = p;
          break;
        }
    }
#if 0
    if(c == NULL)
      {
        std::cerr << "WARNING: You have reached an undefined context  !"<< std::endl;
        std::cerr << "WARNING: Probability distribution for the following context was not defined: " << std::endl;
        for(int k = j; k < i; k++)
          std::cerr << s[k] << " " ;
        std::cerr << std::endl;
        std::cerr << "Position: " << j << " " << i-1 << std::endl;
      }
#endif
    return c;
  }

  std::set <int> ContextTree::getLevelOneNodes()
  {
    std::set<int> result;
    for(int i = 0; i  < (int)_all_context.size(); i++)
      if(_all_context[i]->isLeaf())
        {
          int parent_id = _all_context[i]->getParent();
          if (parent_id < 0)
            continue;
          ContextTreeNodePtr parent = _all_context[parent_id];
          bool levelOne = true;
          for(int l =0; l < (int)_alphabet->size(); l++)
            if((parent->getChild(l) != NULL )&& !parent->getChild(l)->isLeaf())
              levelOne = false;
          if(levelOne)
            result.insert(parent->id());
        }
    return result;
  }


  void ContextTree::removeContextNotUsed()
  {
    std::vector <ContextTreeNodePtr> newAllVector;
    for(int i = 0; i  < (int)_all_context.size(); i++)
      {
        if(_all_context[i]->getParent() >= 0)
          {
            _all_context[i]->setId(newAllVector.size());
            newAllVector.push_back(_all_context[i]);
            if((_all_context[i] != NULL) && (!_all_context[i]->isLeaf()))
              for(int  m = 0; m < (int)_alphabet->size(); m++)
                _all_context[i]->getChild(m)->setParent(_all_context[i]->id());
          }
      }
    _all_context = newAllVector;
  }

  void ContextTree::normalize()
  {
    std::vector <ContextTreeNodePtr> newAllVector;
    for(int i = 0; i  < (int)_all_context.size(); i++)
      {
        double total = 0;
        DoubleVector probs(_alphabet->size());
        for(int l = 0; l < (int)_alphabet->size(); l++)
          total += (double)(_all_context[i]->getCounter())[l];
        for(int l = 0; l < (int)_alphabet->size(); l++){
          probs[l] = (double)((_all_context[i]->getCounter())[l])/total;
        }
        MultinomialDistributionPtr distr = MultinomialDistributionPtr(new MultinomialDistribution(probs));
        distr->setAlphabet(_alphabet);
        _all_context[i]->setDistribution(distr);
      }
  }

    void ContextTree::normalize(ProbabilisticModelPtr old, double pseudocount, int t)
    {
        if(old == NULL){
            std::cerr << "ERROR: ContextTree -> a priori model is null !" << std::endl;
            exit(-1);
        }
    std::vector <ContextTreeNodePtr> newAllVector;
    for(int i = 0; i  < (int)_all_context.size(); i++)
      {
        double total = 0;
        DoubleVector probs(_alphabet->size());

        Sequence s;
        ContextTreeNodePtr current = _all_context[i];
        bool valid = true;
        while (current != getRoot() ) {
            s.push_back (current->symbol());
            if(current->getParent() < 0) {
                valid = false;
                break;
            }
            current = _all_context[current->getParent()];
        }
        if(!valid)
            continue;

        for(int l = 0; l < (int)_alphabet->size(); l++) {
            total += (double)(_all_context[i]->getCounter())[l];
        }
        for(int l = 0; l < (int)_alphabet->size(); l++){
            Sequence s3;
            s3 = s;
            s3.push_back(l);

            double prob = exp(old->evaluatePosition(s3,s3.size()-1, t));

            probs[l] = (double)((_all_context[i]->getCounter())[l] + pseudocount*prob)/(total + pseudocount);
        }

        MultinomialDistributionPtr distr = MultinomialDistributionPtr(new MultinomialDistribution(probs));
        distr->setAlphabet(_alphabet);
        _all_context[i]->setDistribution(distr);
      }
    }

  std::string ContextTree::str() const{
    std::stringstream out;
    std::vector <ContextTreeNodePtr> stack;
    ContextTreeNodePtr current = getRoot();
    printTree(current, out);
    return out.str();
  }

  void ContextTree::initializeCounter(const SequenceEntryList & sequences, int order, const std::map<std::string, double> & weights)
  {
    initializeCounter(sequences, order,0, weights);
  }

  void ContextTree::initializeCounter(const SequenceEntryList & sequences, int order, double pseudocounts, const std::map<std::string, double> & weights)
  {
    if (order < 0) order = 0;

    ContextTreeNodePtr root = createContext();
    if(pseudocounts > 0) {
      for(int sym = 0; sym < root->alphabet_size(); sym++)
        {
          root->setCount(sym,pseudocounts);
        }
    }

    for ( int l = 0; l < (int)sequences.size(); l ++){
      std::string seqname = sequences[l]->getName();
      double weight = 1.0;
      if (weights.find(seqname) != weights.end())
        weight = (weights.find(seqname)->second);
      // std::cerr << seqname << " with weight " << weight << std::endl;
      for( int i = order; i < (int)(sequences[l]->getSequence()).size(); i++)
        {
          int currentSymbol = (sequences[l]->getSequence())[i];
          int j = i - 1;

          ContextTreeNodePtr w = getRoot();

          w->addCount(currentSymbol, weight);

          while((j >= 0) &&  ((i - j) <= order))
            {
              int symbol = (sequences[l]->getSequence())[j];
              if((w->getChild(symbol) == NULL) || w->isLeaf())
                {
                  ContextTreeNodePtr c2 = createContext();
                  w->setChild(c2, symbol);
                }
              w = w->getChild(symbol);

              if(pseudocounts > 0) {
                for(int sym = 0; sym < root->alphabet_size(); sym++)
                  {
                    if(w->getCounter()[sym] <= 0.0)
                      w->setCount(sym,pseudocounts);
                  }
              }


              w->addCount(currentSymbol, weight);
              j -- ;
            }
        }
    }
  }

  void ContextTree::pruneTreeSmallSampleSize(int small_)
  {

    std::set<int> x = getLevelOneNodes();
    std::vector<int> nodesToPrune (x.begin(),x.end());
    std::set<int>::iterator it;

    while(nodesToPrune.size() > 0)
      {
        int id = nodesToPrune.back();
        nodesToPrune.pop_back();
        double total = 0.0;
        ContextTreeNodePtr parentNode = getContext(id);
        if(parentNode->isLeaf())
          break;

        for(int m = 0; m < (int)_alphabet->size(); m++)
          total += (parentNode->getCounter())[m];
        bool foundSmall = false;
        for (int l = 0; l < (int)_alphabet->size(); l++)
          {
            ContextTreeNodePtr childNode = parentNode->getChild(l);
            if(childNode == NULL)
              continue;
            double totalchild = 0;
            for(int m = 0; m < (int)_alphabet->size(); m++)
              {
                totalchild += (childNode->getCounter())[m];
              }
            if(totalchild < small_){
              for(int m = 0; m < (int)_alphabet->size(); m++)
                {
                  (childNode->getCounter())[m] = (parentNode->getCounter())[m];
                }
            }
          }
        if(total < small_)
          {
            parentNode->deleteChildren();
            ContextTreeNodePtr parentNode2 = getContext(parentNode->getParent());
            bool toPrune = true;
            for(int l = 0; l < (int)_alphabet->size(); l++)
              if((parentNode2->getChild(l) != NULL) && !(parentNode2->getChild(l)->isLeaf()))
                {
                  toPrune = false;
                  break;
                }
            if(toPrune)
              nodesToPrune.push_back(parentNode2->id());
          }
      }
  }



  void ContextTree::pruneTree(double delta)
  {

    double sample_size = 0.0;
    for (int l = 0; l < (int)_alphabet->size(); l++)
      sample_size += (getRoot()->getCounter())[l];
    std::set<int> x = getLevelOneNodes();
    std::vector<int> nodesToPrune (x.begin(),x.end());
    std::set<int>::iterator it;
    double small_ = ((double)_alphabet->size())*log(sample_size);

    while(nodesToPrune.size() > 0)
      {
        int id = nodesToPrune.back();
        nodesToPrune.pop_back();
        double total = 0.0;
        double total_diff = 0.0;
        ContextTreeNodePtr parentNode = getContext(id);
        if(parentNode->isLeaf())
          break;

        for(int m = 0; m < (int)_alphabet->size(); m++)
          total += (parentNode->getCounter())[m];
        bool foundSmall = false;
        for (int l = 0; l < (int)_alphabet->size(); l++)
          {
            ContextTreeNodePtr childNode = parentNode->getChild(l);
            double totalChild = 0.0;
            for(int m = 0; m < (int)_alphabet->size(); m++)
              totalChild+= (childNode->getCounter())[m];
            for(int m = 0; m < (int)_alphabet->size(); m++)
              {
                double diff = (double)(parentNode->getCounter())[m] / total;
                diff -= (double)(childNode->getCounter())[m] /totalChild;
                assert(childNode->isLeaf());
                if((double)(childNode->getCounter())[m] < small_)
                  {
                    foundSmall = true;
                    break;
                  }
                if(diff < 0)
                  total_diff -= diff;
                else
                  total_diff += diff;

              }
            if(foundSmall)
              break;
          }
        if((total < small_) ||
           (total_diff <delta) ||
           (foundSmall==true))
          {
            parentNode->deleteChildren();
            ContextTreeNodePtr parentNode2 = getContext(parentNode->getParent());
            bool toPrune = true;
            for(int l = 0; l < (int)_alphabet->size(); l++)
              if((parentNode2->getChild(l) != NULL) && !(parentNode2->getChild(l)->isLeaf()))
                {
                  toPrune = false;
                  break;
                }
            if(toPrune)
              nodesToPrune.push_back(parentNode2->id());
          }
      }
  }





  void ContextTree::initializeContextTreeRissanen(const SequenceEntryList & sequences)
  {
    ContextTreeNodePtr root = createContext();
    for(int i = 0; i < (int)_alphabet->size(); i++)
      root->addCount(i);

    for(int s = 0; s < (int)sequences.size(); s++)
      {
        for(int i = 0; i < (int)(sequences[s]->getSequence()).size(); i++)
          {
            int v = (sequences[s]->getSequence())[i];
            ContextTreeNodePtr w = root;
            if((!w->isLeaf()) && ((w->getCounter())[v] == 1.0))
              {
                for(int l = 0; l < (int)_alphabet->size(); l++)
                  {
                    ContextTreeNodePtr n = w->getChild(l);
                    n->addCount(v);
                  }
                w->addCount(v);
                continue;
              }
            if(w->isLeaf() && ((w->getCounter())[v] == 1.0))
              {
                for(int l = 0; l < (int)_alphabet->size(); l++)
                  {
                    ContextTreeNodePtr n = createContext();
                    n->addCount(v);
                    w->setChild(n, l);
                  }
                w->addCount(v);
                continue;
              }
            int j = i - 1;
            if(j < 0)
              w->addCount(v);
            while(j >= 0)
              {
                int u = (sequences[s]->getSequence())[j];
                w->addCount(v);
                w = w->getChild(u);
                if((!w->isLeaf()) && (w->getCounter())[v] == 1.0)
                  {
                    for(int l = 0; l < (int)_alphabet->size(); l++)
                      {
                        ContextTreeNodePtr n = w->getChild(l);
                        n->addCount(v);
                      }
                    w->addCount(v);
                    break;
                  }
                if(w->isLeaf() && ((w->getCounter())[v] == 1.0) )
                  {
                    for(int l = 0; l  < (int)_alphabet->size(); l++)
                      {
                        ContextTreeNodePtr n = createContext();
                        n->addCount(v);
                        w->setChild(n,l);
                      }
                    w->addCount(v);
                    break;
                  }
                j = j-1;
              }
          }
      }
  }



}
