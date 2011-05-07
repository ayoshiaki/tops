/*
 *       InhomogeneousMarkovChain.cpp
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

#include "InhomogeneousMarkovChain.hpp"
#include "Alphabet.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "Symbol.hpp"
#include "VariableLengthMarkovChainCreator.hpp"

namespace tops{
  //! Evaluate the position i of the sequence s.
  double InhomogeneousMarkovChain::evaluatePosition(const Sequence & s, int i, int t) const{

    assert((t >= 0) && (t < (int)_context_trees.size()));
    ContextTreeNodePtr context = _context_trees[t]->getContext(s,i);
    if(context == NULL) {
        return -HUGE;
    }
    MultinomialDistributionPtr distr = context->getDistribution();
    if(distr == NULL)
        return -HUGE;
    return  distr->evaluatePosition(s,i);
  }

  //! Choose the position i of the sequence s given the subsequence before the position i.
  int InhomogeneousMarkovChain::choosePosition(const Sequence & s, int i, int t) const{
    assert((t >= 0) && (t < (int)_context_trees.size()));
    return  _context_trees[t]->getContext(s,i)->getDistribution()->choosePosition(s,i);
  }

  std::string InhomogeneousMarkovChain::str() const {
    std::stringstream out;
    out << "model_name = \"" << model_name() << "\"" << std::endl;

    for(int i = 0; i < (int)_context_trees.size(); i++)
      {
        out << "p" << i << " = (";
        printDistribution( _context_trees[i], _context_trees[i]->getRoot(), out, alphabet()) ;
        out << ")" << std::endl;
      }
    out << "position_specific_distribution = (";
    if (_context_trees.size() >= 1){
      out << "\"p0\"";
      for(int i = 1; i < (int)_context_trees.size(); i++)
        {
          out <<",\"p" << i << "\"";
        }
    }
    out << ")\n";
    out << "phased =" << _phased << std::endl;
    out << alphabet()->str();
    return out.str();
  }

  int InhomogeneousMarkovChain::size() const
  {
    int size= 0;
    for(int i  = 0; i < (int)_context_trees.size(); i++)
      size += _context_trees[i]->getNumberOfNodes();
    return size;
  }

  void InhomogeneousMarkovChain::printDistribution(ContextTreePtr tree, ContextTreeNodePtr node, std::stringstream & out, AlphabetPtr alphabet) const
    {
      ContextTreeNodePtr current = node;
      std::vector <std::string> aux;
      bool root=true;
      if(current != tree->getRoot() )
        root = false;
      while(current != tree->getRoot())
        {
          aux.push_back(alphabet->getSymbol(current->symbol())->name() );
          current = tree->getContext(current->getParent());
        }
      for(int k = 0; k < (int)alphabet->size(); k++)
        {
          //      if(close(exp(node->getDistribution()->log_probability_of(k)), 0.0, 1e-100))
          //        continue;
          out << "\"" << alphabet->getSymbol(k)->name() << "\" | " ;
          if(root)
            out << "\"\" " ;
          else
            {
              out << "\"";
              out << aux[aux.size() -1];
              for(int i = aux.size()-2; i >=0; i--)
                out << " " << aux[i] ;
              out << "\"";
            }
          out << ": " ;
          out << exp(node->getDistribution()->log_probability_of(k)) << ";";
          if(node->isLeaf())
            out << " # leaf";
          out << std::endl;

        }
      if(!node->isLeaf())
        for(int l = 0; l < node->alphabet_size(); l++)
          if(node->getChild(l) != NULL)
            printDistribution(tree, node->getChild(l), out, alphabet);
    }
  void InhomogeneousMarkovChain::removeSequenceFromModel(const Sequence & s,  int phase){
    for(int i = 0; i < (int)s.size()-1; i++)
      phase = mod(phase + 1, maximumTimeValue()+1);
    ContextTreeNodePtr c =  _context_trees[phase]->getRoot();
    ContextTreeNodePtr p;
    int j;
    for(j = s.size()-2; j >=0; j--){
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
    for(; j >= 0; j--) {
      for(int l = 0; l < (int)alphabet()->size(); l++) {
        ContextTreeNodePtr n = _context_trees[phase]->createContext();
        DoubleVector prob;
        for(int i = 0; i < n->alphabet_size(); i++)
          prob.push_back(exp(c ->getDistribution()->log_probability_of(i)));
        MultinomialDistributionPtr distr = MultinomialDistributionPtr(new MultinomialDistribution(prob));
        n->setDistribution(distr);
        c->setChild(n, l);
      }
      c = c->getChild(s[j]);
    }

    ContextTreeNodePtr current = _context_trees[phase]->getContext(s,s.size()-1);


    current->getDistribution()->log_probability_of(s[s.size()-1], -HUGE);
    double sum = 0;
    for(int symbol = 0; symbol < alphabet()->size(); symbol++)
        {
            sum += exp(current->getDistribution()->log_probability_of(symbol));
        }
    for(int symbol = 0; symbol < alphabet()->size(); symbol++)
        {
            if(symbol != s[s.size()-1]) {
                double x = exp(current->getDistribution()->log_probability_of(symbol));
                current->getDistribution()->log_probability_of(symbol, log(x/sum));
            }
        }


    std::vector <ContextTreeNodePtr> children = current->getChildren();
    if(current->isLeaf())
      return;
    std::vector <ContextTreeNodePtr> stack;
    for(int i = 0 ; i < (int)children.size(); i++)
        stack.push_back(children[i]);
#if 1
    while(stack.size() > 0) {
      current = stack.back();
      stack.pop_back();
      if(current == NULL)
        continue;
      assert(current->getDistribution() != NULL);



    current->getDistribution()->log_probability_of(s[s.size()-1], -HUGE);
    double sum = 0;
    for(int symbol = 0; symbol < alphabet()->size(); symbol++)
        {
            sum += exp(current->getDistribution()->log_probability_of(symbol));
        }
    for(int symbol = 0; symbol < alphabet()->size(); symbol++)
        {
            if(symbol != s[s.size()-1]) {
                double x = exp(current->getDistribution()->log_probability_of(symbol));
                current->getDistribution()->log_probability_of(symbol, log(x/sum));
            }
        }
      if(current->isLeaf())
        continue;
      children = current->getChildren();
      for(int i = 0 ; i < (int)children.size(); i++)
        stack.push_back(children[i]);
    }
#endif




  }
  ProbabilisticModelParameters InhomogeneousMarkovChain::parameters() const {
    ProbabilisticModelParameters par;
    par.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name())));
    StringVector parnames;
    for(int i = 0; i <(int)_context_trees.size();i++)
      {
        std::stringstream out;
        out << "p" << i;
        parnames.push_back(out.str());
      }
    par.add("position_specific_distribution" ,
            StringVectorParameterValuePtr(new StringVectorParameterValue(parnames)));
    for(int i = 0; i <(int)_context_trees.size();i++)
      {
        std::stringstream out;
        out << "p" << i;
        par.add(out.str(), _context_trees[i]->getParameterValue());
      }
    par.add("phased", IntParameterValuePtr(new IntParameterValue(_phased)));
    par.add("alphabet", alphabet()->getParameterValue());
    return par;
  }
  void InhomogeneousMarkovChain::initialize(const ProbabilisticModelParameters & parameters)  {
    ProbabilisticModelParameterValuePtr symbols = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr position_specific_model_names = parameters.getMandatoryParameterValue("position_specific_distribution");
    ProbabilisticModelParameterValuePtr p = parameters.getMandatoryParameterValue("phased");

    bool is_phased = (p->getInt() == 1);


    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromVector(symbols->getStringVector());
    std::vector <ContextTreePtr> position_specific_trees;
    StringVector model_names = position_specific_model_names->getStringVector();
    position_specific_trees.resize(model_names.size());
    for(int i = 0; i < (int)model_names.size(); i++)
      {
        ProbabilisticModelParameters vlmc;
        ProbabilisticModelParameterValuePtr probs = parameters.getMandatoryParameterValue(model_names[i]);
        if (probs == NULL)
          {
            std::cerr << "ERROR:  Could not find " << model_names[i] << std::endl;
          }

        vlmc.add("alphabet", symbols);
        vlmc.add("probabilities", probs);

        VariableLengthMarkovChainCreator vlmcCreator;
        VariableLengthMarkovChainPtr mc = vlmcCreator.createVLMC(vlmc);
        position_specific_trees[i] = mc->getTree();
      }
    phased(is_phased);
    setAlphabet(alphabet);
    setPositionSpecificDistribution(position_specific_trees);
  }
}

