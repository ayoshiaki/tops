/*
 *       VariableLengthMarkovChain.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      Ígor Bonadio <ibonadio@ime.usp.br>
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

#include "VariableLengthMarkovChain.hpp"
#include "VariableLengthMarkovChainCreator.hpp"
#include "Symbol.hpp"
#include <boost/algorithm/string.hpp>
namespace tops {

    std::string VariableLengthMarkovChain::print_graph () const {
        std::stringstream out;
        AlphabetPtr alphabet = ProbabilisticModel::alphabet();
        int nnodes = _tree->getNumberOfNodes();
        for (int i = 0; i < nnodes; i++) {
            ContextTreeNodePtr current = _tree->getContext(i);
            ContextTreeNodePtr root = _tree->getRoot();
            std::vector <std::string> aux;
            while(current != root) {
                aux.push_back(alphabet->getSymbol(current->symbol())->name() );
                current = _tree->getContext(current->getParent());
            }
            std::stringstream node_id;
            for (int j = aux.size() - 1; j >= 0; j--)
                node_id  << aux[j] ;
            out << _tree->getContext(i)->id() << " " <<  node_id.str() << std::endl;
        }
        out << "#" << std::endl;
        for (int i = 0; i < nnodes; i++) {
            out << _tree->getContext(i)->id() << " " <<  _tree->getContext(_tree->getContext(i)->getParent())->id()  << std::endl;
        }
        return out.str();
    }


  void VariableLengthMarkovChain::printDistribution(ContextTreePtr tree, ContextTreeNodePtr node, std::stringstream & out, AlphabetPtr alphabet) const {
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


  double VariableLengthMarkovChain::evaluatePosition(const Sequence & s, unsigned int i)const{
    ContextTreeNodePtr c = _tree->getContext(s,i);
    if (c == NULL)
      return -HUGE;
    else
      return c->getDistribution()->evaluatePosition(s,i);
  }

  double VariableLengthMarkovChain::choosePosition(const Sequence & s, int i)const{
    ContextTreeNodePtr c = _tree->getContext(s,i);
    if (c == NULL)
      return -HUGE;
    else
      return c->getDistribution()->choosePosition(s,i);
  }

  ProbabilisticModelCreatorPtr VariableLengthMarkovChain::getFactory() const {
    return VariableLengthMarkovChainCreatorPtr (new VariableLengthMarkovChainCreator());
  }

  int VariableLengthMarkovChain::size() const {
    return (alphabet()->size() -1)* _tree->getNumberOfNodes();
  }

  std::string VariableLengthMarkovChain::str() const {
    std::stringstream out;
    if(_tree == NULL){
      std::cerr << "This Markov chain model was not defined !" << std::endl;
      return out.str();
    }

    out << "model_name = \"" << model_name() << "\"" << std::endl;
      out << "probabilities = (";
      printDistribution( _tree, _tree->getRoot(), out, alphabet()) ;
      out << ")" << std::endl;
      out << alphabet()->str() ;
      return out.str();
  }

  ProbabilisticModelParameters VariableLengthMarkovChain::parameters() const
  {
    ProbabilisticModelParameters p;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue("VariableLengthMarkovChain")));
    p.add("probabilities", _tree->getParameterValue());
    p.add("alphabet", alphabet()->getParameterValue());
    return p;
  }

  void VariableLengthMarkovChain::initialize(const ProbabilisticModelParameters & p) {
    ProbabilisticModelParameterValuePtr probs = p.getMandatoryParameterValue("probabilities");
    ProbabilisticModelParameterValuePtr symbols = p.getMandatoryParameterValue("alphabet");
    if((probs == NULL) || (symbols == NULL))
      {
        return;
      }
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromVector(symbols->getStringVector());
    ContextTreePtr tree =
      ContextTreePtr(new ContextTree(alphabet));
    ContextTreeNodePtr root = tree->createContext();

    std::string  root_str("");
    setTree(tree);
    setAlphabet(alphabet);

    map<std::string,DoubleVector> probmap;
    std::map <std::string,double>::const_iterator it;
    std::map <std::string,std::vector<double> >::const_iterator it2;
    for(it = (probs->getDoubleMap()).begin();
        it !=(probs->getDoubleMap()).end();
        it++)
      {
        std::vector <std::string> splited;
        boost::regex separator("\\|");
        tops::split_regex(it->first, splited, separator);
        if(splited.size() == 1) {
          splited.push_back("");
        }

        std::string context (splited[1]);
        std::string symbol ( splited[0]);

        if(probmap.find(context) == probmap.end())
          {
            int id = alphabet->getSymbol(symbol)->id();
            DoubleVector probs;
            probs.resize(alphabet->size());
            probmap[context]=probs;
            if(id < (int)probmap[context].size())
              (probmap[context])[id] = it->second;
          }
        else
          {
            int id = alphabet->getSymbol(symbol)->id();
            if(id < (int)probmap[context].size())
              (probmap[context])[id] = it->second;
          }
      }

    for(it2 = probmap.begin(); it2 != probmap.end(); it2++) {
        std::vector <std::string> history;
        std::string context = it2->first;
        std::vector<double> p = it2->second;
        boost::split(history, context, boost::is_any_of(" "));
        if(context.size() <= 0)
          {
            ProbabilisticModelParameters fddpar ;
            fddpar.add("probabilities", DoubleVectorParameterValuePtr(new DoubleVectorParameterValue(p)));
            fddpar.add("alphabet", alphabet->getParameterValue());
            DiscreteIIDModelPtr distr =
              DiscreteIIDModelPtr(new DiscreteIIDModel());
            distr->initialize(fddpar);
            root->setDistribution(distr);
          }
        else
          {
            ContextTreeNodePtr current  = root;
            // history with indices -1, -2, -3,...
            for(int i = 0; i < (int)history.size() ;i++)
              {
                int id = alphabet->getSymbol(history[i])->id();
                if(id < 0)
                  {
                    std::cerr << "ERROR: Symbol, '" << history[i] << "', not in the alphabet !" << std::endl;
                    exit(-1);
                  }

                if(current->getChild(id) == NULL)
                  {
                    ContextTreeNodePtr node2 = tree->createContext();
                    current->setChild(node2, id);
                    if(i == ((int)history.size() - 1))
                      {
                        ProbabilisticModelParameters fddpar ;
                        fddpar.add("probabilities", DoubleVectorParameterValuePtr(new DoubleVectorParameterValue(p)));
                        fddpar.add("alphabet", alphabet->getParameterValue());
                        DiscreteIIDModelPtr distr =
                          DiscreteIIDModelPtr(new DiscreteIIDModel());
                        distr->initialize(fddpar);
                        node2->setDistribution(distr);
                      }
                    else
                      {
                        DoubleVector probs2;
                        for(int i = 0; i < (int)alphabet->size()-1; i++)
                          probs2.push_back(1.0/(double)alphabet->size());
                        ProbabilisticModelParameters fddpar ;
                        fddpar.add("probabilities", DoubleVectorParameterValuePtr(new DoubleVectorParameterValue(probs2)));
                        fddpar.add("alphabet", alphabet->getParameterValue());
                        DiscreteIIDModelPtr distr =
                          DiscreteIIDModelPtr(new DiscreteIIDModel());
                        distr->initialize(fddpar);
                        node2->setDistribution(distr);
                      }
                  }
                current = current->getChild(id);
              }
          }
      }

  }

  void VariableLengthMarkovChain::removeSequenceFromModel(const Sequence & s,  int phase){
      ContextTreeNodePtr c =  _tree->getRoot();
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
        ContextTreeNodePtr n = _tree->createContext();
        DoubleVector prob;
        for(int i = 0; i < n->alphabet_size(); i++)
          prob.push_back(exp(c ->getDistribution()->log_probability_of(i)));
        DiscreteIIDModelPtr distr = DiscreteIIDModelPtr(new DiscreteIIDModel(prob));
        n->setDistribution(distr);
        c->setChild(n, l);
      }
      c = c->getChild(s[j]);
    }

    ContextTreeNodePtr current = _tree->getContext(s,s.size()-1);


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

}
