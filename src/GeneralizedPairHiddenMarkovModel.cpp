#include "Alphabet.hpp"
#include "GeneralizedPairHiddenMarkovModel.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "util.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdio.h>
#include <algorithm>

namespace tops {

  double GeneralizedPairHiddenMarkovModel::forward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    alpha.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
    }

    //Initialization
    for(int k = 0; k < nstates; k++){
      alpha[k](0,0) = initialProbabilities()->log_probability_of(k);
    }

    //Recursion
    for (int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
        for(int k = 0; k < nstates; k++){
          if(i == 0 && j == 0)
            continue;
    alpha[k](i,j) = -HUGE;
          int maxS1 = getState(k)->maxSeq1();
          int minS1 = getState(k)->minSeq1();
          int maxS2 = getState(k)->maxSeq2();
          int minS2 = getState(k)->minSeq2();
    if(i - minS1 < 0 || j - minS2 < 0){
      continue;
    }
    for(int ne1 = minS1; ne1 <= maxS1; ne1++){
      if(i - ne1 < 0)
        break;
      for(int ne2 = minS2; ne2 <= maxS2; ne2++){
        if(j - ne2 < 0)
    break;
        if(ne1 == 0 && ne2 == 0)
    continue;
        double aux = -HUGE;
        if(i - ne1 == 0 && j - ne2 == 0){
    aux = alpha[k](i-ne1,j-ne2);
    aux += getState(k)->duration()->log_probability_of_pair(ne1,ne2);
    if(maxS1 == 1 && maxS2 == 1)
      aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]);
    if(maxS1 == 1 && maxS2 == 0)
      aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
    if(maxS1 == 0 && maxS2 == 1)
      aux += getState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]);
    if(maxS1 > 1 && maxS2 > 1){
      vector<Matrix> f;
      aux += getState(k)->emission()->pairDecodable()->forward(sub_seq(seq1, i-ne1, i-1), sub_seq(seq2, j-ne2, j-1), f);
    }
    alpha[k](i,j) = log_sum(alpha[k](i,j), aux);
    continue;
        }
        for(int l = 0; l < (int)(getState(k)->iTransitions()).size(); l++){
    int id = getState(k)->getITransId(l);
    aux = log_sum(aux, alpha[id](i-ne1,j-ne2) + getState(id)->transitions()->log_probability_of(k));
        }
        aux += getState(k)->duration()->log_probability_of_pair(ne1,ne2);
        if(maxS1 == 1 && maxS2 == 1)
    aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]);
        if(maxS1 == 1 && maxS2 == 0)
    aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
        if(maxS1 == 0 && maxS2 == 1)
    aux += getState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]);
        if(maxS1 > 1 && maxS2 > 1){
    vector<Matrix> f;
    aux += getState(k)->emission()->pairDecodable()->forward(sub_seq(seq1, i-ne1, i-1), sub_seq(seq2, j-ne2, j-1), f);
        }
        alpha[k](i,j) = log_sum(alpha[k](i,j), aux);
      }
    }
  }
      }
    }

    //Termination
    double sum = -HUGE;
    for(int i = 0; i < nstates; i++){
      alpha[i](length1,length2) += getState(i)->transitions()->log_probability_of(_end_id);
      sum = log_sum(sum, alpha[i](length1,length2));
    }
    a = alpha;
    return sum;
  }

  double GeneralizedPairHiddenMarkovModel::viterbi(const Sequence & seq1, const Sequence & seq2, Sequence & statePath, Sequence & alignment1, Sequence & alignment2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    vector<vMatrix> pathMatrix;
    alpha.resize(nstates);
    pathMatrix.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
      (pathMatrix[i]).resize(length1+1,length2+1);
    }

    //Initialization
    for(int k = 0; k < nstates; k++){
      alpha[k](0,0) = initialProbabilities()->log_probability_of(k);
    }

    //Recursion
    for (int i = 0; i <= length1; i++){
      for(int j = 0; j <= length2; j++){
        for(int k = 0; k < nstates; k++){
          if(i == 0 && j == 0)
            continue;
          int maxS1 = getState(k)->maxSeq1();
          int minS1 = getState(k)->minSeq1();
          int maxS2 = getState(k)->maxSeq2();
          int minS2 = getState(k)->minSeq2();
    alpha[k](i,j) = -HUGE;
    if(i - minS1 < 0 || j - minS2 < 0){
      (pathMatrix[k](i,j)).lastState = -1;
      continue;
    }
    for(int ne1 = minS1; ne1 <= maxS1; ne1++){
      if(i - ne1 < 0)
        break;
      for(int ne2 = minS2; ne2 <= maxS2; ne2++){
        if(j - ne2 < 0)
    break;
        if(ne1 == 0 && ne2 == 0)
    continue;
        double aux = -HUGE;
        Sequence saux1, saux2, saux3;
        vector<Matrix> f;
        int state;
        if(i - ne1 == 0 && j - ne2 == 0){
    aux = alpha[k](i-ne1,j-ne2);
    aux += getState(k)->duration()->log_probability_of_pair((sub_seq(seq1, i-ne1, i-1)).size(),(sub_seq(seq2, j-ne2, j-1)).size());
    if(maxS1 == 1 && maxS2 == 1){
      aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]);
      saux1.push_back(seq1[i-1]);
      saux2.push_back(seq2[j-1]);
    }
    if(maxS1 == 1 && maxS2 == 0){
      aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
      saux1.push_back(seq1[i-1]);
      saux2.push_back(_gap_id);
    }
    if(maxS1 == 0 && maxS2 == 1){
      aux += getState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]);
      saux1.push_back(_gap_id);
      saux2.push_back(seq2[j-1]);
    }
    if(maxS1 > 1 && maxS2 > 1){
      aux += getState(k)->emission()->pairDecodable()->viterbi(sub_seq(seq1, i-ne1, i-1), sub_seq(seq2, j-ne2, j-1), saux3, saux1, saux2, f);
    }
    if(aux > alpha[k](i,j)){
      alpha[k](i,j) = aux;
      (pathMatrix[k](i,j)).lastState = -1;
      (pathMatrix[k](i,j)).ne1 = ne1;
      (pathMatrix[k](i,j)).ne2 = ne2;
      (pathMatrix[k](i,j)).al1 = saux1;
      (pathMatrix[k](i,j)).al2 = saux2;
    }
    continue;
        }
        for(int l = 0; l < (int)(getState(k)->iTransitions()).size(); l++){
    int id = getState(k)->getITransId(l);
    if(aux < alpha[id](i-ne1,j-ne2) + getState(id)->transitions()->log_probability_of(k)){
      aux = alpha[id](i-ne1,j-ne2) + getState(id)->transitions()->log_probability_of(k);
      state = id;
    }
        }
        aux += getState(k)->duration()->log_probability_of_pair((sub_seq(seq1, i-ne1, i-1)).size(),(sub_seq(seq2, j-ne2, j-1)).size());
        if(maxS1 == 1 && maxS2 == 1){
    aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],seq2[j-1]);
    saux1.push_back(seq1[i-1]);
    saux2.push_back(seq2[j-1]);
        }
        if(maxS1 == 1 && maxS2 == 0){
    aux += getState(k)->emission()->log_probability_of_pair(seq1[i-1],_gap_id);
    saux1.push_back(seq1[i-1]);
    saux2.push_back(_gap_id);
        }
        if(maxS1 == 0 && maxS2 == 1){
    aux += getState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1]);
    saux1.push_back(_gap_id);
    saux2.push_back(seq2[j-1]);
        }
        if(maxS1 > 1 && maxS2 > 1){
    aux += getState(k)->emission()->pairDecodable()->viterbi(sub_seq(seq1, i-ne1, i-1), sub_seq(seq2, j-ne2, j-1), saux3, saux1, saux2, f);
        }
        if(aux > alpha[k](i,j)){
    alpha[k](i,j) = aux;
    (pathMatrix[k](i,j)).lastState = state;
    (pathMatrix[k](i,j)).ne1 = ne1;
    (pathMatrix[k](i,j)).ne2 = ne2;
    (pathMatrix[k](i,j)).al1 = saux1;
    (pathMatrix[k](i,j)).al2 = saux2;
        }
      }
    }
  }
      }
    }

    //Termination
    double max = -HUGE;
    int lastState = -1;
    for(int i = 0; i < nstates; i++){
      alpha[i](length1,length2) += getState(i)->transitions()->log_probability_of(_end_id);
      if(alpha[i](length1,length2) > max){
  max = alpha[i](length1,length2);
  lastState = i;
      }
    }

    //Traceback
    int i = length1;
    int j = length2;
    Sequence al1,al2,path;
    while(lastState != -1){
      alignment1.insert(alignment1.end(),((pathMatrix[lastState](i,j)).al1).begin(),((pathMatrix[lastState](i,j)).al1).end());
      alignment2.insert(alignment2.end(),((pathMatrix[lastState](i,j)).al2).begin(),((pathMatrix[lastState](i,j)).al2).end());
      int alsize = ((pathMatrix[lastState](i,j)).al1).size();
      for(int k = 0; k < alsize; k++)
  statePath.push_back(lastState);
      int ne1 = (pathMatrix[lastState](i,j)).ne1;
      int ne2 = (pathMatrix[lastState](i,j)).ne2;
      lastState = (pathMatrix[lastState](i,j)).lastState;
      i -= ne1;
      j -= ne2;
    }
    a = alpha;
    return max;
  }

  double GeneralizedPairHiddenMarkovModel::backward(const Sequence & seq1, const Sequence & seq2, vector<Matrix> &a)
  {
    int nstates = _states.size();
    int length1 = seq1.size();
    int length2 = seq2.size();
    vector<Matrix> alpha;
    alpha.resize(nstates);
    for(int i = 0; i < nstates; i++){
      (alpha[i]).resize(length1+1,length2+1);
    }

    //Initialization
    for(int k = 0; k < nstates; k++){
      alpha[k](length1,length2) = getState(k)->transitions()->log_probability_of(_end_id);
    }

    //Recursion
    for (int i = length1; i >= 0; i--){
      for(int j = length2; j >= 0; j--){
        for(int k = 0; k < nstates; k++){

          if(i == length1 && j == length2)
            continue;

    alpha[k](i,j) = -HUGE;
    if(i == 0 && j == 0){
      int maxS1 = getState(k)->maxSeq1();
      int minS1 = getState(k)->minSeq1();
      int maxS2 = getState(k)->maxSeq2();
      int minS2 = getState(k)->minSeq2();
      for(int ne1 = minS1; ne1 <= maxS1; ne1++){
        if(i + ne1 > length1)
    break;
        for(int ne2 = minS2; ne2 <= maxS2; ne2++){
    if(j + ne2 > length2)
      break;
    if(ne1 == 0 && ne2 == 0)
      continue;
    if(maxS1 == 1 && maxS2 == 1)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[k](i+ne1,j+ne2) +
            initialProbabilities()->log_probability_of(k) +
            getState(k)->emission()->log_probability_of_pair(seq1[i-1+ne1],seq2[j-1+ne2]));
    else if(maxS1 == 1 && maxS2 == 0)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[k](i+ne1,j+ne2) +
            initialProbabilities()->log_probability_of(k)+
            getState(k)->emission()->log_probability_of_pair(seq1[i-1+ne1],_gap_id));
    else if(maxS1 == 0 && maxS2 == 1)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[k](i+ne1,j+ne2) +
            initialProbabilities()->log_probability_of(k)+
            getState(k)->emission()->log_probability_of_pair(_gap_id,seq2[j-1+ne2]));
    else if(maxS1 > 1 && maxS2 > 1){
      vector<Matrix> b;
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[k](i+ne1,j+ne2) +
            initialProbabilities()->log_probability_of(k) +
            getState(k)->duration()->log_probability_of_pair(ne1,ne2)+
            getState(k)->emission()->pairDecodable()->forward(sub_seq(seq1, i, i+ne1-1),sub_seq(seq2, j, j+ne2-1),b));
    }
        }
      }
      continue;
    }

    for(int l = 0; l < (int)(getState(k)->oTransitions()).size(); l++){
      int id = getState(k)->getOTransId(l);
      int maxS1 = getState(id)->maxSeq1();
      int minS1 = getState(id)->minSeq1();
      int maxS2 = getState(id)->maxSeq2();
      int minS2 = getState(id)->minSeq2();
      for(int ne1 = minS1; ne1 <= maxS1; ne1++){
        if(i + ne1 > length1)
    break;
        for(int ne2 = minS2; ne2 <= maxS2; ne2++){
    if(j + ne2 > length2)
      break;
    if(ne1 == 0 && ne2 == 0)
      continue;
    if(maxS1 == 1 && maxS2 == 1)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[id](i+ne1,j+ne2) +
            getState(k)->transitions()->log_probability_of(id) +
            getState(id)->emission()->log_probability_of_pair(seq1[i-1+ne1],seq2[j-1+ne2]));
    else if(maxS1 == 1 && maxS2 == 0)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[id](i+ne1,j+ne2) +
            getState(k)->transitions()->log_probability_of(id)+
            getState(id)->emission()->log_probability_of_pair(seq1[i-1+ne1],_gap_id));
    else if(maxS1 == 0 && maxS2 == 1)
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[id](i+ne1,j+ne2) +
            getState(k)->transitions()->log_probability_of(id)+
            getState(id)->emission()->log_probability_of_pair(_gap_id,seq2[j-1+ne2]));
    else if(maxS1 > 1 && maxS2 > 1){
      vector<Matrix> b;
      alpha[k](i,j) = log_sum(alpha[k](i,j),
            alpha[id](i+ne1,j+ne2) +
            getState(k)->transitions()->log_probability_of(id)+
            getState(id)->duration()->log_probability_of_pair(ne1,ne2)+
            getState(id)->emission()->pairDecodable()->forward(sub_seq(seq1, i, i+ne1-1),sub_seq(seq2, j, j+ne2-1),b));
    }
        }
      }
    }
        }
      }
    }

    //Termination
    double sum = -HUGE;
    for(int i = 0; i < nstates; i++){
      sum = log_sum(sum, alpha[i](0,0));
    }
    a = alpha;
    return sum;
  }

  void GeneralizedPairHiddenMarkovModel::initialize(const ProbabilisticModelParameters & parameters) {
    ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
    ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
    ProbabilisticModelParameterValuePtr number_of_emissions = parameters.getMandatoryParameterValue("number_of_emissions");
    ProbabilisticModelParameterValuePtr initial_probabilities = parameters.getMandatoryParameterValue("initial_probabilities");
    ProbabilisticModelParameterValuePtr end_probabilities = parameters.getMandatoryParameterValue("end_probabilities");
    ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
    ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");
    ProbabilisticModelParameterValuePtr durations = parameters.getOptionalParameterValue("duration_probabilities");

    ProbabilisticModelCreatorClient creator;

    AlphabetPtr states = AlphabetPtr(new Alphabet());
    states->initializeFromVector(state_names->getStringVector());

    _end_id = states->size();

    AlphabetPtr observations = AlphabetPtr(new Alphabet());
    observations->initializeFromVector(observation_symbols->getStringVector());
    if(observations->has("_"))
      _gap_id = observations->getSymbol("_")->id();
    else{
      observations->createSymbol(std::string("_"));
      _gap_id = observations->getSymbol("_")->id();
    }

    DiscreteIIDModelPtr init = DiscreteIIDModelPtr(new DiscreteIIDModel());
    init->initializeFromMap(initial_probabilities->getDoubleMap(), states);

    std::map<std::string,std::string> emissModels = emissions->getStringMap();

    std::map<std::string,std::string> durModels = durations->getStringMap();

    std::map<std::string,std::string> numEmiss = number_of_emissions->getStringMap();

    std::map<std::string,double> transpar = transitions->getDoubleMap();
    std::map<std::string,double> endMap = end_probabilities->getDoubleMap();
    std::map<std::string,double>::const_iterator transparit;
    std::map<std::string,DoubleVector> trans;
    std::map<std::string,IntVector> inTrans;
    std::map<std::string,IntVector> outTrans;
    for(transparit = transpar.begin(); transparit != transpar.end(); transparit++){
      std::vector<std::string> splited;
      boost::regex separator("\\|");
      split_regex(transparit->first, splited, separator);
      if(splited.size() == 1)
        splited.push_back("");

      std::string to(splited[0]);
      std::string from(splited[1]);
      if(trans.find(from) == trans.end())
        {
          int id = states->getSymbol(to)->id();
          DoubleVector probs;
          IntVector outgoing;
          outgoing.push_back(id);
          outTrans[from] = outgoing;
          probs.resize(states->size() + 1);
    trans[from]=probs;
    if(endMap.find(from) != endMap.end())
      trans[from][_end_id] = endMap[from];
    else
      trans[from][_end_id] = 0.0;
    if(id < (int)trans[from].size())
            trans[from][id] = transparit->second;
        }
      else
        {
          int id = states->getSymbol(to)->id();
          outTrans[from].push_back(id);
          if(id < (int)trans[from].size())
            trans[from][id] = transparit->second;
        }
      if(inTrans.find(to) == inTrans.end()){
        int id = states->getSymbol(from)->id();
        IntVector incoming;
        incoming.push_back(id);
        inTrans[to] = incoming;
      }
      else{
        int id = states->getSymbol(from)->id();
        inTrans[to].push_back(id);
      }
    }

    std::vector<PairDecodableStatePtr> states_list;

    for(int i = 0; i < (int)states->size(); i++){
      SymbolPtr state_name = states->getSymbol(i);
      DiscreteIIDModelPtr t;
      IntVector itr, otr;
      ProbabilisticModelPtr dur, emiss;
      int maxS1, minS1, maxS2, minS2;

      std::map<std::string,DoubleVector>::const_iterator strdvit = trans.find(state_name->name());
      if(strdvit != trans.end())
  t = DiscreteIIDModelPtr(new DiscreteIIDModel(strdvit->second));
      else{
  cerr << "State " << state_name->name() << " has no transitions." << endl;
  exit(-1);
      }

      std::map<std::string,IntVector>::const_iterator strivit = inTrans.find(state_name->name());
      if(strivit != inTrans.end())
  itr = strivit->second;
      else{
  cerr << "State " << state_name->name() << " has no incoming transitions." << endl;
  exit(-1);
      }

      strivit = outTrans.find(state_name->name());
      if(strivit != outTrans.end())
  otr = strivit->second;
      else{
  cerr << "State " << state_name->name() << " has no transitions coming out of it." << endl;
  exit(-1);
      }

      std::map<std::string,std::string>::const_iterator strmapit = durModels.find(state_name->name());
      if(strmapit != durModels.end()){
  std::string model_file = strmapit->second;
  ProbabilisticModelPtr model = creator.create(model_file);
  if(model == NULL)
    {
      cerr << "Could not create model from file " << model_file << endl;
      exit(-1);
    }
  dur = model;
      }
      else
  dur = ConstantModelPtr(new ConstantModel());

      strmapit = emissModels.find(state_name->name());
      if(strmapit != emissModels.end()){
  std::string model_file = strmapit->second;
  ProbabilisticModelPtr model = creator.create(model_file);
  if(model == NULL)
    {
      cerr << "Could not create model from file " << model_file << endl;
      exit(-1);
    }
  emiss = model;
      }
      else{
  cerr << "Emission probabilities for state " << state_name->name() << " could not be initialized." << endl;
  exit(-1);
      }

      strmapit = numEmiss.find(state_name->name());
      if(strmapit != numEmiss.end()){
  std::vector<std::string> spl1;
  std::vector<std::string> spl2;
  std::vector<std::string> spl3;
  boost::regex sep1(",");
  boost::regex sep2("-");
  split_regex(strmapit->second, spl1, sep1);
  split_regex(spl1[0], spl2, sep2);
  split_regex(spl1[1], spl3, sep2);
  minS1 = atoi(spl2[0].c_str());
  maxS1 = atoi(spl2[1].c_str());
  minS2 = atoi(spl3[0].c_str());
  maxS2 = atoi(spl3[1].c_str());
      }
      else{
  cerr << "Number of emissions for state " << state_name->name() << " could not be initialized." << endl;
  exit(-1);
      }

      GPHMMStatePtr statePtr = GPHMMStatePtr(new GPHMMState(i, state_name, emiss, t, dur, itr, otr, maxS1, minS1, maxS2, minS2));
      states_list.push_back(statePtr);
    }

    setStates(states_list, states);
    setObservationSymbols(observations);
    setInitialProbabilities(init);
  }
}
