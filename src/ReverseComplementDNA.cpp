/*
 *       ReverseComplementDNA.cpp
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

#include "ProbabilisticModelCreatorClient.hpp"
#include "ConfigurationReader.hpp"
#include "ReverseComplementDNA.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
namespace tops {

  void ReverseComplementDNA::revcomp(Sequence & revCompSeq, const Sequence & s, int begin,  int end) const{
    AlphabetPtr a = subModel()->alphabet();
    if(end >= (int)s.size())
      end = s.size()-1;
    if(begin >= (int)s.size())
      begin = s.size()-1;

    for(int i = end; i >= begin; i--) {
      SymbolPtr symbol1 = a->getSymbol(s[i]);
      SymbolPtr revSymbol = revAlphabet->getSymbol(symbol1->name());
      revCompSeq.push_back(revSymbol->id());
    }
  }

  double ReverseComplementDNA::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const{
    Sequence revCompSeq;
    revcomp(revCompSeq, s, (int)begin, (int)end);
    double result = subModel()->evaluate(revCompSeq, begin, end);
    return result;
  }


  Sequence & ReverseComplementDNA::choose(Sequence & h, int size) const {
    Sequence revCompSeq;
    ProbabilisticModelDecorator::choose(revCompSeq, size);
    revcomp(h, revCompSeq, 0, revCompSeq.size()-1);
    return h;
  }


  Sequence & ReverseComplementDNA::choose(Sequence &h, int initial_phase, int size) const{
    Sequence revCompSeq;
    ProbabilisticModelDecorator::choose(revCompSeq, initial_phase, size);
    revcomp(h, revCompSeq, 0, size-1);
    return h;
  }


  double ReverseComplementDNA::prefix_sum_array_compute(int begin , int end){
    int b = _seqLength - end - 1;
    int e = _seqLength - begin - 1;
    double r = ProbabilisticModelDecorator::prefix_sum_array_compute(b, e);
    return r;
  }
  double ReverseComplementDNA::prefix_sum_array_compute(int begin , int end, int phase){
    int b = _seqLength - end - 1;
    int e = _seqLength - begin - 1;
    double r = ProbabilisticModelDecorator::prefix_sum_array_compute(b, e, phase);
    return r;
  }

  bool ReverseComplementDNA::initialize_prefix_sum_array(const Sequence & s, int phase){
    Sequence revCompSeq;
    revcomp(revCompSeq, s, 0, s.size()-1);
    _seqLength = s.size();
    return ProbabilisticModelDecorator::initialize_prefix_sum_array(revCompSeq, phase);
  }

  bool ReverseComplementDNA::initialize_prefix_sum_array(const Sequence & s) {
    Sequence revCompSeq;
    revcomp(revCompSeq, s, 0, s.size()-1);
    _seqLength = s.size();
    return ProbabilisticModelDecorator::initialize_prefix_sum_array(revCompSeq);
  }
  std::string ReverseComplementDNA::model_name () const {
    return "ReverseComplementDNA";
  }

  std::string ReverseComplementDNA::str() const{

    std::stringstream out;
    out << "model_name =\"" << model_name() << "\"" << std::endl;
    std::string modelname = ProbabilisticModelDecorator::subModelName();
    if(modelname.length() > 0)
      out << "model = " << modelname << std::endl ;
    else
      out << "model = [" << subModel()->str() << "]" << std::endl ;
    return out.str();
  }


  ProbabilisticModelParameters ReverseComplementDNA::parameters() const
  {
    ProbabilisticModelParameters p ;
    p.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name())));
    std::string modelname = ProbabilisticModelDecorator::subModelName();
    if(modelname.length() > 0)
      p.add("model", StringParameterValuePtr( new StringParameterValue(modelname)));
    else
      p.add("model", ProbabilisticModelParameterListValuePtr (new ProbabilisticModelParameterListValue(subModel()->parameters())));
    return p;
  }

  void ReverseComplementDNA::initialize(const ProbabilisticModelParameters & p)
  {
    ProbabilisticModelParameterValuePtr modelpar = p.getMandatoryParameterValue("model");
    ProbabilisticModelCreatorClient creator;
    ConfigurationReader reader;
    std::string modelstr = modelpar->getString();
    ProbabilisticModelPtr m ;
    if((modelstr.size() > 0) && (modelstr[0] == '[') ){
      modelstr = modelstr.substr(1, modelstr.size() -2 );
      reader.load(modelstr);
      ProbabilisticModelParametersPtr par = reader.parameters();
      m = creator.create(*par);
    } else {
      m = creator.create(modelstr) ;
      if(m == NULL) {
        std::cerr << "Can not load model file " << modelstr<< "!" << std::endl;
        exit(-1);
      }
    }
    setSubModel(m);
    setAlphabet(m->alphabet());
  }

}
