#include "MultipleAlignment.hpp"

using namespace std;

namespace tops{

  void MultipleAlignment::clearAll(){
    _graph.clear();
    _seqPosMap.clear();
    _postProbs.clear();
    _alignment.clear();
  }   

  void MultipleAlignment::computeAllAlignments(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, string alFileName, string outDir){\
    boost::timer::cpu_timer t;
    int avg = 0;
    for(int i = 0; i < (int)seqs.size(); i++){
      avg += seqs[i].size();
    }
    avg /= seqs.size();
    cerr << alFileName << endl << "number of sequences: " << seqs.size() << endl << "average size: " << avg << endl;
    _seqs = seqs;
    _names = names;
    
    cerr << "\tComputing posterior probabilities...";
    t.resume();
    postProbAlign(almodel,_ppAlign,_ppGap1,_ppGap2,_eas);
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;

    map<string,map<string,SparseMatrixPtr > > consPPAlign;
    map<string,map<string,SparseMatrixPtr > > consPPGap1;
    map<string,map<string,SparseMatrixPtr > > consPPGap2;

    cerr << "\tNo consistency." << endl;
    cerr << "\t\tGenerating alignment with modified sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	_ppAlign[_names[i]][_names[j]]->addGaps(_ppGap1[_names[i]][_names[j]], _ppGap2[_names[i]][_names[j]]);
      }
    }
    initializePostProbsList(_ppAlign);  
    generateGraph();
    generateAlignment(almodel);
    string outFile = "";
    outFile.append(outDir);
    outFile.append("C0");
    outFile.append(alFileName);
    ofstream fout;
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;

    clearAll();

    cerr << "\t\tGenerating alignment with original sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	_ppAlign[_names[i]][_names[j]]->removeLastLine();
	_ppAlign[_names[i]][_names[j]]->removeLastColumn();
      }
    }
    initializePostProbsList(_ppAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C1");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;

    clearAll();

    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppAlign[_names[i]][_names[j]]));
	consPPGap1[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppGap1[_names[i]][_names[j]]));
	consPPGap2[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppGap2[_names[i]][_names[j]]));
      }
    }

    cerr << "\tApplying consistency transformation with weights and gaps...";
    t.resume();
    predalignAlConsistencyWithEas(consPPAlign, consPPGap1, consPPGap2, numit, _eas);
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;
    cerr << "\t\tGenerating alignment with modified sequence annealing...";
    t.resume();
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C2");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;

    clearAll();

    cerr << "\t\tGenerating alignment original sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]]->removeLastLine();
	consPPAlign[_names[i]][_names[j]]->removeLastColumn();
      }
    }
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C3");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;

    clearAll();

    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppAlign[_names[i]][_names[j]]));
	consPPGap1[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppGap1[_names[i]][_names[j]]));
	consPPGap2[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppGap2[_names[i]][_names[j]]));
      }
    }

    cerr << "\tApplying consistency transformation without weights and with gaps...";
    t.resume();
    predalignAlConsistencyNoEas(consPPAlign, _ppGap1, _ppGap2, numit);
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;
    cerr << "\t\tGenerating alignment with modified sequence annealing...";
    t.resume();
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C4");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;

    clearAll();

    cerr << "\t\tGenerating alignment original sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]]->removeLastLine();
	consPPAlign[_names[i]][_names[j]]->removeLastColumn();
      }
    }
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C5");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;

    clearAll();

    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_ppAlign[_names[i]][_names[j]]));
	consPPGap1[_names[i]][_names[j]].reset();
	consPPGap2[_names[i]][_names[j]].reset();
      }
    }
    
    cerr << "\tApplying picxaa consistency transformation...";
    t.resume();
    picxaaAlConsistency(consPPAlign, _eas, numit);
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;
    cerr << "\t\tGenerating alignment with original sequence annealing...";
    t.resume();
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C6");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;

    clearAll();

    cerr << "\t\tGenerating alignment with modified sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]]->addGaps(_ppGap1[_names[i]][_names[j]], _ppGap2[_names[i]][_names[j]]);
      }
    }
    initializePostProbsList(consPPAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C7");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;

    clearAll();
    
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	consPPAlign[_names[i]][_names[j]].reset();
      }
    }

    cerr << "\tApplying classic consistency transformation...";
    t.resume();
    classicAlConsistency(_ppAlign, numit);
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;
    cerr << "\t\tGenerating alignment with original sequence annealing...";
    t.resume();
    initializePostProbsList(_ppAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C8");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl;

    clearAll();

    cerr << "\t\tGenerating alignment with modified sequence annealing...";
    t.resume();
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	_ppAlign[_names[i]][_names[j]]->addGaps(_ppGap1[_names[i]][_names[j]], _ppGap2[_names[i]][_names[j]]);
      }
    }
    initializePostProbsList(_ppAlign);  
    generateGraph();
    generateAlignment(almodel);
    outFile = "";
    outFile.append(outDir);
    outFile.append("C9");
    outFile.append(alFileName);
    fout.open(outFile.c_str());
    for(int i = 0; i < (int)_seqs.size(); i++){
      fout << _alignment[i];
    }
    fout << endl;
    fout.close();
    cerr << "Done! (" << boost::timer::format(t.elapsed()) << "s)" << endl << endl;
  }

  void MultipleAlignment::computeOneAlignment(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, int consScheme){
    _seqs = seqs;
    _names = names;

    postProbAlign(almodel,_ppAlign,_ppGap1,_ppGap2,_eas);

    if(consScheme == 1)
      predalignAlConsistencyWithEas(_ppAlign,_ppGap1,_ppGap2,numit,_eas);

    if(consScheme == 2){
      predalignAlConsistencyWithEas(_ppAlign,_ppGap1,_ppGap2,numit,_eas);
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  _ppAlign[_names[i]][_names[j]]->removeLastLine();
	  _ppAlign[_names[i]][_names[j]]->removeLastColumn();
	}
      }
    }

    if(consScheme == 3)
      picxaaAlConsistency(_ppAlign,_eas,numit);
    
    if(consScheme == 4)
      classicAlConsistency(_ppAlign,numit);

    /*if(_alpha != -1)
    predalConsistencies(predmodels,ppAlign);*/

    initializePostProbsList(_ppAlign);  
    cerr << "Number of stored posterior probabilities: " << _postProbs.size() << endl;
    generateGraph();
    generateAlignment(almodel);
    for(int i = 0; i < (int)_seqs.size(); i++){
      cout << _alignment[i];
    }
    cout << endl;
  }

  void MultipleAlignment::computePredictionsAndAlignment(ProbabilisticModelPtr almodel, vector<ProbabilisticModelPtr> &predmodels, SequenceList seqs, vector<string> names, int numit, bool alternateConsistencies, string outDir){
    _seqs = seqs;
    _names = names;

    postProbAlign(almodel,_ppAlign,_ppGap1,_ppGap2,_eas);
    cerr << "Passou do postProbAlign" << endl;
    postProbPred(predmodels, _ppPred);
    cerr << "Passou do postProbPred" << endl;

    if(alternateConsistencies){
      for(int r = 0; r < numit; r++){
	makeAlignmentConsistentWithPrediction(_ppPred,_ppAlign,1);
	makePredictionConsistentWithAlignment(_ppPred,_ppAlign,1);
      }
    }
    else{
      map<string, SparseMatrixPtr> consistentPPpred;
      for(int i = 0; i < (int)_names.size(); i++){
	consistentPPpred[_names[i]] = SparseMatrixPtr(new SparseMatrix(_ppPred[_names[i]]));
      }
      makePredictionConsistentWithAlignment(consistentPPpred,_ppAlign,numit);
      makeAlignmentConsistentWithPrediction(_ppPred,_ppAlign,numit);
      for(int i = 0; i < (int)_names.size(); i++){
	_ppPred[_names[i]].reset();
	_ppPred[_names[i]] = consistentPPpred[_names[i]];
      }
    }

    for(int i = 0; i < (int)_names.size(); i++){
      SequenceFormatManager::instance()->setFormat(SequenceFormatPtr(new SequenceFormat()));
      SequenceEntry output(predmodels[i]->decodable()->getStateNames());
      Sequence states;
      clock_t begin = clock();
      float ea = predmodels[i]->decodable()->MEAPred(_seqs[i], states, _ppPred[_names[i]]);
      clock_t end = clock();
      std::cerr << "TIME: " << (double)(end - begin)/CLOCKS_PER_SEC << std::endl;
      stringstream new_name;
      new_name << _names[i] <<  ": " << ea;
      output.setName(new_name.str());
      output.setSequence(states);
      string outFile = "";
      outFile.append(outDir);
      outFile.append(_names[i]);
      outFile.append(".pred");
      ofstream fout;
      fout.open(outFile.c_str());
      fout << output;
      fout.close();
    }
      
    initializePostProbsList(_ppAlign);  
    cerr << "Number of stored posterior probabilities: " << _postProbs.size() << endl;
    generateGraph();
    generateAlignment(almodel);
    for(int i = 0; i < (int)_seqs.size(); i++){
      cout << _alignment[i];
    }
    cout << endl;
  }

  /*void MultipleAlignment::initializeFromFile(ProbabilisticModelPtr almodel, SequenceList seqs, vector<string> names, int numit, int consScheme, string inputFileName){
    _seqs = seqs;
    _names = names;

    ifstream fin;
    fin.open(inputFileName.c_str());

    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	string temp, name1, name2;
	int line, col, nEntries;
	float prob;
	_ppAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_seqs[i].size(),_seqs[j].size()));
	_ppGap1[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_seqs[i].size(),_seqs[j].size()));
	_ppGap2[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(_seqs[i].size(),_seqs[j].size()));
	fin >> name1 >> name2 >> temp;
	fin >> prob;
	if(temp.compare("ea") != 0 || name1.compare(_names[i]) != 0 || name2.compare(_names[j]) != 0){
	  cerr << "temp = >" << temp << "< name1 = >" << name1 << "< name2 = >" << name2 << "<" << endl;
	  cerr << "_namesi = >" << _names[i] << "< _namesj = >" << _names[j] << "<" << endl;
	  cerr << inputFileName << " is not in the correct format, or does not correspond the the sequence file provided. ea" << endl;
	  exit(-1);
	}
	_eas[name1][name2] = prob;
	fin >> name1 >> name2 >> temp;
	if(temp.compare("ppAlign") != 0 || name1.compare(_names[i]) != 0 || name2.compare(_names[j]) != 0){
	  cerr << inputFileName << " is not in the correct format, or does not correspond the the sequence file provided. ppAlign" << endl;
	  exit(-1);
	}
	fin >> nEntries;
	for(int k = 0; k < nEntries; k++){
	  fin >> line >> col >> prob;
	  _ppAlign[name1][name2]->add(line,col,prob);
	}
	fin >> name1 >> name2 >> temp;
	if(temp.compare("ppGap1") != 0 || name1.compare(_names[i]) != 0 || name2.compare(_names[j]) != 0){
	  cerr << inputFileName << " is not in the correct format, or does not correspond the the sequence file provided. ppGap1" << endl;
	  exit(-1);
	}
	fin >> nEntries;
	for(int k = 0; k < nEntries; k++){
	  fin >> line >> col >> prob;
	  _ppGap1[name1][name2]->add(line,col,prob);
	}
	fin >> name1 >> name2 >> temp;
	if(temp.compare("ppGap2") != 0 || name1.compare(_names[i]) != 0 || name2.compare(_names[j]) != 0){
	  cerr << inputFileName << " is not in the correct format, or does not correspond the the sequence file provided.ppGap2" << endl;
	  exit(-1);
	}
	fin >> nEntries;
	for(int k = 0; k < nEntries; k++){
	  fin >> line >> col >> prob;
	  _ppGap2[name1][name2]->add(line,col,prob);
	}
      }
    }
    if(consScheme == 1)
      predalignAlConsistencyWithEas(_ppAlign, _ppGap1, _ppGap2,numit,_eas);

    if(consScheme == 2){
      predalignAlConsistencyWithEas(_ppAlign,_ppGap1,_ppGap2,numit,_eas);
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  _ppAlign[_names[i]][_names[j]]->removeLastLine();
	  _ppAlign[_names[i]][_names[j]]->removeLastColumn();
	}
      }
    }

    if(consScheme == 3)
      picxaaAlConsistency(_ppAlign,_eas,numit);
    
    if(consScheme == 4)
      classicAlConsistency(_ppAlign,numit);

    initializePostProbsList(_ppAlign);  
    generateGraph();
    generateAlignment(almodel);
    for(int i = 0; i < (int)_seqs.size(); i++){
      cout << _alignment[i];
    }
    cout << endl;
    }*/

  /*  void MultipleAlignment::trainAndComputePPs(string initialModelFile, SequenceList seqs, vector<string> names, int maxTrainIter, string outFile){
    int numseqs = seqs.size();
    ProbabilisticModelCreatorClient creator;
    ProbabilisticModelPtr model;
    ofstream fout;
    fout.open(outFile.c_str());
    if(!fout.is_open()){
      cerr << "Could not open file: " << outFile << endl; 
      exit(-1);
    }

    model = creator.create(initialModelFile);
    model->pairDecodable()->trainBaumWelch(seqs, maxTrainIter, 1e-5);
    for(int i = 0; i < numseqs; i++){
      for(int j = i+1; j < numseqs; j++){
	SparseMatrixPtr ppAlign, ppGap1, ppGap2;
	ostringstream temp;
	float ea = model->pairDecodable()->posteriorProbabilities(seqs[i],seqs[j],ppAlign, ppGap1, ppGap2);
	fout << names[i] << " " << names[j] << " ea" << endl << ea << endl;
	fout << names[i] << " " << names[j] << " ppAlign" << endl;
	ppAlign->printMatrix(temp);
	fout << temp.str();
	temp.str("");
	fout << names[i] << " " << names[j] << " ppGap1" << endl;
	ppGap1->printMatrix(temp);
	fout << temp.str();
	temp.str("");
	fout << names[i] << " " << names[j] << " ppGap2" << endl;
	ppGap2->printMatrix(temp);
	fout << temp.str();
	temp.str("");
      }
    }
    fout.close();
    }*/

  //////////////////////////////////////////////////
  //Posterior probabilities calculation functions///
  //////////////////////////////////////////////////

  float MultipleAlignment::postProbAlign(ProbabilisticModelPtr model, map<string, map< string, SparseMatrixPtr > > &ppAlign, map<string, map< string, SparseMatrixPtr > > &ppGap1, map<string, map< string, SparseMatrixPtr > > &ppGap2, map<string, map<string, float> > &eas)
  {
    float sum = 0;
    for(int i = 0; i < (int)_seqs.size(); i++){
      for(int j = i+1; j < (int)_seqs.size(); j++){
	float ea = model->pairDecodable()->posteriorProbabilities(_seqs[i],_seqs[j],ppAlign[_names[i]][_names[j]], ppGap1[_names[i]][_names[j]], ppGap2[_names[i]][_names[j]]);
	eas[_names[i]][_names[j]] = ea;
	sum += ea;
      }
    }
   
    return sum;
  }

  void MultipleAlignment::postProbPred(vector<ProbabilisticModelPtr> &predmodels, map<string, SparseMatrixPtr > &ppPred)
  {  
    for(int i = 0; i < (int)_seqs.size(); i++){
      fMatrix postPred;
      predmodels[i]->decodable()->posteriorProbabilities(_seqs[i],postPred);
      ppPred[_names[i]] = SparseMatrixPtr(new SparseMatrix());
      ppPred[_names[i]]->buildPredMatrix(postPred.size1(),postPred.size2(),postPred);
    }
  }


  ////////////////////////////////////////////
  //Consistency transformation functions//////
  ////////////////////////////////////////////
    
  void MultipleAlignment::picxaaAlConsistency(map< string, map< string, SparseMatrixPtr > > &ppAlign,  map<string, map<string, float> > eas, int numit){
    for(int r = 0; r < numit; r++){
      map< string, map< string, SparseMatrixPtr > > consistentPPAlign;
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  float sumea = 2.0*eas[_names[i]][_names[j]]*eas[_names[i]][_names[j]];
	  fMatrix postMatch;
	  ppAlign[_names[i]][_names[j]]->getfMatrixTimesX(postMatch, 2.0*eas[_names[i]][_names[j]]*eas[_names[i]][_names[j]]);
	  for(int m = 0; m < (int)_names.size(); m++){
	    if(m == i || m ==j)
	      continue;
	    else if(m < i){
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postMatch, (eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]));
	      sumea += (eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]);
	    }
	    else if(m > i && m < j){
	      ppAlign[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postMatch, (eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]));
	      sumea += (eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]);
	    }
	    else if(m > j){
	      SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix());
	      AUX->transposeOf(ppAlign[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX, postMatch, (eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]));
	      AUX.reset();
	      sumea += (eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]);
	    }
	  }
	  consistentPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postMatch,ppAlign[_names[i]][_names[j]], sumea));
	}	
      }
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  ppAlign[_names[i]][_names[j]].reset();
	  ppAlign[_names[i]][_names[j]] = consistentPPAlign[_names[i]][_names[j]];
	}
      }
    }
  }

  void MultipleAlignment::classicAlConsistency(map< string, map< string, SparseMatrixPtr > > &ppAlign, int numit){
    for(int r = 0; r < numit; r++){
      map< string, map< string, SparseMatrixPtr > > consistentPPAlign;
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  fMatrix postMatch;
	  ppAlign[_names[i]][_names[j]]->getfMatrixTimesX(postMatch, 2.0);
	  for(int m = 0; m < (int)_names.size(); m++){
	    if(m == i || m ==j)
	      continue;
	    else if(m < i){
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postMatch);
	    }
	    else if(m > i && m < j){
	      ppAlign[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postMatch);
	    }
	    else if(m > j){
	      SparseMatrixPtr AUX = SparseMatrixPtr(new SparseMatrix());
	      AUX->transposeOf(ppAlign[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX, postMatch);
	      AUX.reset();
	    }
	  }
	  consistentPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postMatch,ppAlign[_names[i]][_names[j]], (float)_names.size()));
	}	
      }
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  ppAlign[_names[i]][_names[j]].reset();
	  ppAlign[_names[i]][_names[j]] = consistentPPAlign[_names[i]][_names[j]];
	}
      }
    }
  }

  void MultipleAlignment::predalignAlConsistencyNoEas(map< string, map< string, SparseMatrixPtr > > &ppAlign, map< string, map< string, SparseMatrixPtr > > &ppGap1,map< string, map< string, SparseMatrixPtr > > &ppGap2, int numit){

    for(int r = 0; r < numit; r++){
      map< string, map< string, SparseMatrixPtr > > consistentPPAlign;
      map< string, map< string, SparseMatrixPtr > > consistentPPGap1;
      map< string, map< string, SparseMatrixPtr > > consistentPPGap2;
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  fMatrix postMatch;
	  fMatrix postGap1;
	  fMatrix postGap2;
	  ppAlign[_names[i]][_names[j]]->getfMatrixTimesX(postMatch, 2.0);
	  ppGap1[_names[i]][_names[j]]->getfMatrixTimesX(postGap1, 1.0);
	  ppGap2[_names[i]][_names[j]]->getfMatrixTimesX(postGap2, 1.0);
	  for(int m = 0; m < (int)_names.size(); m++){
	    if(m == i || m == j)
	      continue;
	    else if(m < i){
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postMatch);
	      ppGap1[_names[m]][_names[i]]->leftTransXright(ppGap1[_names[m]][_names[j]], postMatch);
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppGap2[_names[m]][_names[j]], postGap2);
	      ppGap2[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postGap1);
	    }
	    else if(m > i && m < j){
	      ppAlign[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postMatch);
	      ppGap2[_names[i]][_names[m]]->leftXright(ppGap1[_names[m]][_names[j]], postMatch);
	      ppAlign[_names[i]][_names[m]]->leftXright(ppGap2[_names[m]][_names[j]], postGap2);
	      ppGap1[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postGap1);
	    }
	    else if(m > j){
	      SparseMatrixPtr AUX1 = SparseMatrixPtr(new SparseMatrix());
	      SparseMatrixPtr AUX2 = SparseMatrixPtr(new SparseMatrix());
	      SparseMatrixPtr AUX3 = SparseMatrixPtr(new SparseMatrix());
	      AUX1->transposeOf(ppAlign[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX1, postMatch);
	      ppGap1[_names[i]][_names[m]]->leftXright(AUX1, postGap1);
	      AUX1.reset();
	      AUX2->transposeOf(ppGap2[_names[j]][_names[m]]);
	      ppGap2[_names[i]][_names[m]]->leftXright(AUX2, postMatch);
	      AUX2.reset();
	      AUX3->transposeOf(ppGap1[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX3, postGap2);
	      AUX3.reset();
	    }
	  }
	  consistentPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postMatch,ppAlign[_names[i]][_names[j]], (float)_names.size()));
	  consistentPPGap1[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postGap1,ppGap1[_names[i]][_names[j]], (float)(_names.size()-1)));
	  consistentPPGap2[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postGap2,ppGap2[_names[i]][_names[j]], (float)(_names.size()-1)));
	}	
      }
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  ppAlign[_names[i]][_names[j]].reset();
	  ppAlign[_names[i]][_names[j]] = consistentPPAlign[_names[i]][_names[j]];
	  ppGap1[_names[i]][_names[j]].reset();
	  ppGap1[_names[i]][_names[j]] = consistentPPGap1[_names[i]][_names[j]];
	  ppGap2[_names[i]][_names[j]].reset();
	  ppGap2[_names[i]][_names[j]] = consistentPPGap2[_names[i]][_names[j]];
	}
      }
    }
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	ppAlign[_names[i]][_names[j]]->addGaps(ppGap1[_names[i]][_names[j]], ppGap2[_names[i]][_names[j]]);
      }
    }
  }

  void MultipleAlignment::predalignAlConsistencyWithEas(map< string, map< string, SparseMatrixPtr > > &ppAlign, map< string, map< string, SparseMatrixPtr > > &ppGap1,map< string, map< string, SparseMatrixPtr > > &ppGap2, int numit, map<string, map<string, float> > eas){

    for(int r = 0; r < numit; r++){
      map< string, map< string, SparseMatrixPtr > > consistentPPAlign;
      map< string, map< string, SparseMatrixPtr > > consistentPPGap1;
      map< string, map< string, SparseMatrixPtr > > consistentPPGap2;
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  fMatrix postMatch;
	  fMatrix postGap1;
	  fMatrix postGap2;
	  float sumea = 2.0*eas[_names[i]][_names[j]]*eas[_names[i]][_names[j]];
	  ppAlign[_names[i]][_names[j]]->getfMatrixTimesX(postMatch, 2.0*eas[_names[i]][_names[j]]*eas[_names[i]][_names[j]]);
	  ppGap1[_names[i]][_names[j]]->getfMatrixTimesX(postGap1, 1.0);
	  ppGap2[_names[i]][_names[j]]->getfMatrixTimesX(postGap2, 1.0);
	  for(int m = 0; m < (int)_names.size(); m++){
	    if(m == i || m == j)
	      continue;
	    else if(m < i){
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postMatch, eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]);
	      ppGap1[_names[m]][_names[i]]->leftTransXright(ppGap1[_names[m]][_names[j]], postMatch, eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]);
	      ppAlign[_names[m]][_names[i]]->leftTransXright(ppGap2[_names[m]][_names[j]], postGap2, eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]);
	      ppGap2[_names[m]][_names[i]]->leftTransXright(ppAlign[_names[m]][_names[j]], postGap1, eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]]);
	      sumea += eas[_names[m]][_names[i]]*eas[_names[m]][_names[j]];
	    }
	    else if(m > i && m < j){
	      ppAlign[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postMatch, eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]);
	      ppGap2[_names[i]][_names[m]]->leftXright(ppGap1[_names[m]][_names[j]], postMatch, eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(ppGap2[_names[m]][_names[j]], postGap2, eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]);
	      ppGap1[_names[i]][_names[m]]->leftXright(ppAlign[_names[m]][_names[j]], postGap1, eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]]);
	      sumea += eas[_names[i]][_names[m]]*eas[_names[m]][_names[j]];
	    }
	    else if(m > j){
	      SparseMatrixPtr AUX1 = SparseMatrixPtr(new SparseMatrix());
	      SparseMatrixPtr AUX2 = SparseMatrixPtr(new SparseMatrix());
	      SparseMatrixPtr AUX3 = SparseMatrixPtr(new SparseMatrix());
	      AUX1->transposeOf(ppAlign[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX1, postMatch, eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]);
	      ppGap1[_names[i]][_names[m]]->leftXright(AUX1, postGap1, eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]);
	      AUX1.reset();
	      AUX2->transposeOf(ppGap2[_names[j]][_names[m]]);
	      ppGap2[_names[i]][_names[m]]->leftXright(AUX2, postMatch, eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]);
	      AUX2.reset();
	      AUX3->transposeOf(ppGap1[_names[j]][_names[m]]);
	      ppAlign[_names[i]][_names[m]]->leftXright(AUX3, postGap2, eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]]);
	      AUX3.reset();
	      sumea += eas[_names[i]][_names[m]]*eas[_names[j]][_names[m]];
	    }
	  }
	  consistentPPAlign[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postMatch,ppAlign[_names[i]][_names[j]], sumea));
	  consistentPPGap1[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postGap1,ppGap1[_names[i]][_names[j]], sumea));
	  consistentPPGap2[_names[i]][_names[j]] = SparseMatrixPtr(new SparseMatrix(postGap2,ppGap2[_names[i]][_names[j]], sumea));
	}	
      }
      for(int i = 0; i < (int)_names.size(); i++){
	for(int j = i+1; j < (int)_names.size(); j++){
	  ppAlign[_names[i]][_names[j]].reset();
	  ppAlign[_names[i]][_names[j]] = consistentPPAlign[_names[i]][_names[j]];
	  ppGap1[_names[i]][_names[j]].reset();
	  ppGap1[_names[i]][_names[j]] = consistentPPGap1[_names[i]][_names[j]];
	  ppGap2[_names[i]][_names[j]].reset();
	  ppGap2[_names[i]][_names[j]] = consistentPPGap2[_names[i]][_names[j]];
	}
      }
    }
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	ppAlign[_names[i]][_names[j]]->addGaps(ppGap1[_names[i]][_names[j]], ppGap2[_names[i]][_names[j]]);
      }
    }
  }

  void MultipleAlignment::makeAlignmentConsistentWithPrediction(map<string, SparseMatrixPtr > &ppPred, map< string, map< string, SparseMatrixPtr > > &ppAlign, int numit){
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	ppAlign[_names[i]][_names[j]]->applyPrediction(ppPred[_names[i]], ppPred[_names[j]]);
      }
    }
    classicAlConsistency(ppAlign,numit);
  }

  void MultipleAlignment::makePredictionConsistentWithAlignment(map<string, SparseMatrixPtr > &ppPred, map< string, map<string, SparseMatrixPtr> > &ppAlign, int numit){
    for(int r = 0; r < numit; r++){
      map< string, SparseMatrixPtr > consistentPPpred;
      for(int i = 0; i < (int)_names.size(); i++){
	fMatrix postPred;
	ppPred[_names[i]]->getfMatrixTimesX(postPred, 1.0);
	for(int m = 0; m < (int)_names.size(); m++){
	  if(m == i)
	    continue;
	  else if(m < i){
	    ppAlign[_names[m]][_names[i]]->leftTransXright(ppPred[_names[m]], postPred);
	  }
	  else{
	    ppAlign[_names[i]][_names[m]]->leftXright(ppPred[_names[m]], postPred);
	  }
	}
	consistentPPpred[_names[i]] = SparseMatrixPtr(new SparseMatrix(postPred,ppPred[_names[i]], (float)_names.size()));
      }	
      for(int i = 0; i < (int)_names.size(); i++){
	ppPred[_names[i]].reset();
	ppPred[_names[i]] = consistentPPpred[_names[i]];
      }
    }
  }     


  ///////////////////////////////////////////////////
  //Alignment construction functions///////////////////////////
  ///////////////////////////////////////////////////

  bool compare(postProb first, postProb second){
    if(first.prob >= second.prob)
      return true;
    return false;
  }

  void MultipleAlignment::initializePostProbsList(map< string, map< string, SparseMatrixPtr > > &ppAlign){
    for(int i = 0; i < (int)_names.size(); i++){
      for(int j = i+1; j < (int)_names.size(); j++){
	bool hasElement = true;
	while(hasElement){
	  int s1, p1, s2, p2;
	  postProb p;
	  s1 = i;
	  s2 = j;
	  hasElement = ppAlign[_names[i]][_names[j]]->next(&p1,&p2,&(p.prob));
	  p.pos1.first = s1;
	  p.pos1.second = p1-1;
	  p.pos2.first = s2;
	  p.pos2.second = p2-1;
	  /*p.prob = (p.prob)/(ppAlign[_names[i]][_names[j]]->get(p1,_seqs[j].size())+ppAlign[_names[i]][_names[j]]->get(_seqs[i].size(),p2));
	  if(p.prob < 0.5){
	    continue;
	  }
	  if(p1 == (int)_seqs[s1].size() || p2 == (int)_seqs[s2].size())
	  continue;*/
	  //cout << "s1 = " << s1 << " p1 = " << p1 << " s2 = " << s2 << " p2 = " << p2 << endl;
	  _postProbs.push_back(p);
	}
      }
    }
    _postProbs.sort(compare);
  }

  void MultipleAlignment::generateAlignment(ProbabilisticModelPtr almodel){
    vector<Sequence> als;
    for(int i = 0; i < (int)_seqs.size(); i++){
      Sequence s(_graph.size(),almodel->pairDecodable()->getGapId());
      als.push_back(s);
    }
    for(int i = 0; i < (int)_graph.size(); i++){
      for(int k = 0; k < (int)_graph[i].colSeqPos.size(); k++){
	if(_graph[i].colSeqPos[k].second == (int)_seqs[_graph[i].colSeqPos[k].first].size())
	  continue;
	als[_graph[i].colSeqPos[k].first][i] = _seqs[_graph[i].colSeqPos[k].first][_graph[i].colSeqPos[k].second];
      }
    }
    for(int i = 0; i < (int)_seqs.size(); i++){
      SequenceEntry se(almodel->alphabet());
      se.setName(_names[i]);
      se.setSequence(als[i]);
      _alignment.push_back(se);
    }
  }



  void MultipleAlignment::generateGraph(){
    //Initialize all possible columns
    _numberOfColumns = 0;
    int numSeqs = _seqs.size();
    int maxSeqSize = 0;
    for(int i = 0; i < numSeqs; i++){
      if((int)_seqs[i].size() > maxSeqSize)
	maxSeqSize = _seqs[i].size();
    }

    int index = 0;
    for(int j = 0; j < maxSeqSize; j++){
      for(int i = 0; i < numSeqs; i++){
	if(j >= (int)_seqs[i].size())
	  continue;
	seqPos sp;
	column c;
	sp.first = i;
	sp.second = j;
	c.index = index;
	_seqPosMap[sp] = index;
	index++;
	c.isDead = false;
	c.visited = false;
	c.colSeqPos.push_back(sp);
	_graph.push_back(c);
	_numberOfColumns++;
      }
    }

    while(!_postProbs.empty()){
      addToGappedGraph(*_postProbs.begin());
      _postProbs.erase(_postProbs.begin());
    }
    findGraphMcd();
  }

  void MultipleAlignment::addToGraph(postProb p){
    int index1 = _seqPosMap[p.pos1];
    int index2 = _seqPosMap[p.pos2];
    int ub,lb;
    seqPos ubsp, lbsp;
    if(index1 > index2){
      ub = index1;
      ubsp = p.pos1;
      lb = index2;
      lbsp = p.pos2;
    }
    else if(index2 > index1){
      ub = index2;
      ubsp = p.pos2;
      lb = index1;
      lbsp = p.pos1;
    }
    else
      return;

    vector<column> df,db;
    _graph[ub].colSeqPos.push_back(lbsp);
    bool cycle = dfsf(ub,_graph[lb],df);
    unvisit(df);
    if(cycle){
      _graph[ub].colSeqPos.pop_back();
      return;
    }
    dfsb(lb,_graph[ub],db);
    unvisit(db);
    if(db[0].index != ub)
      cerr << "DEU PAUU!!" << endl;
    if(df[0].index != lb)
      cerr << "DEU PAUU!!" << endl;
    assert(db[0].index == ub);
    assert(df[0].index == lb);
    merge(&db[0],&df[0]);
    reorder(df,db);    
  }

  bool MultipleAlignment::dfsf(int ub, column n, vector<column> &df){
    _graph[n.index].visited = true;
    df.push_back(n);
    for(int i = 0; i < (int)n.colSeqPos.size(); i++){
      seqPos w = n.colSeqPos[i];
      w.second++;
      if(w.second >= (int)_seqs[w.first].size())
	continue;
      int next_index = _seqPosMap[w];
      if(next_index == ub){
	return true;
      }
      if(next_index > ub || _graph[next_index].visited)
	continue;
      bool cycle = dfsf(ub,_graph[next_index],df);
      if(cycle){
	return true;
      }
    }
    return false;
  }

  void MultipleAlignment::dfsb(int lb, column n, vector<column> &db){
    _graph[n.index].visited = true;
    db.push_back(n);
    for(int i = 0; i < (int)n.colSeqPos.size(); i++){
      seqPos w = n.colSeqPos[i];
      if(w.second == (int)_seqs[w.first].size())
	continue;
      w.second--;
      if(w.second < 0)
	continue;
      int next_index = _seqPosMap[w];
      if(next_index < lb || _graph[next_index].visited)
	continue;
      dfsb(lb,_graph[next_index],db);
    }
  }

  void MultipleAlignment::merge(column *ub, column *lb){
    lb->isDead = true;
    _numberOfColumns--;
    bool flag = false;
    for(int i = 0; i < (int)(lb->colSeqPos).size(); i++){
      for(int j = 0; j < (int)(ub->colSeqPos).size(); j++){
	if(lb->colSeqPos[i].first == ub->colSeqPos[j].first && lb->colSeqPos[i].second == ub->colSeqPos[j].second)
	  flag = true;
      }
      if(flag == false)
	ub->colSeqPos.push_back(lb->colSeqPos[i]);
      flag = false;
    }
    lb->colSeqPos.clear();
  }
    
  bool smallerIndex(column a, column b){
    if(a.index < b.index)
      return true;
    return false;
  }

  bool biggerIndex(column a, column b){
    if(a.index >= b.index)
      return true;
    return false;
  }

  void MultipleAlignment::reorder(vector<column> &df, vector<column> &db){
    vector<int> L;
    int i = 0;
    int j = 0;
    sort(df.begin(), df.end(), smallerIndex);
    sort(db.begin(), db.end(),smallerIndex);
    for(int k = 0; k < (int)(db.size()+df.size()); k++){
      if(i >= (int)db.size()){
	L.push_back(df[j].index);
	j++;
	continue;
      }
      if(j >= (int)df.size()){
	L.push_back(db[i].index);
	i++;
	continue;
      }
      if(db[i].index < df[j].index){
	L.push_back(db[i].index);
	i++;
      }
      else{
	L.push_back(df[j].index);
	j++;
      }
    }
    i = 0;
    for(int k = 0; k < (int)db.size(); k++){
      db[k].index = L[i];
      for(j = 0; j < (int)db[k].colSeqPos.size(); j++){
	_seqPosMap[db[k].colSeqPos[j]] = L[i];
      }
      _graph[L[i]] = db[k];
      i++;
    }
    for(int k = 0; k < (int)df.size(); k++){
      df[k].index = L[i];
      for(j = 0; j < (int)df[k].colSeqPos.size(); j++){
	_seqPosMap[df[k].colSeqPos[j]] = L[i];
      }
      _graph[L[i]] = df[k];
      i++;
    }
  }

  void MultipleAlignment::findGraphMcd(){
    //find nodes with first positions in sequences
    //unvisit();
    std::stack< column* > nodeStack;
    vector<bool> inStack (_graph.size(),false);
    vector<int> finishingTimes (_graph.size(),-1);
    vector<int> initialColumns;
    for(int i = 0; i < (int)_seqs.size(); i++){
      seqPos sp;
      sp.first = i;
      sp.second = 0;
      if(!inStack[_seqPosMap[sp]]){
	initialColumns.push_back(_seqPosMap[sp]);
	inStack[_seqPosMap[sp]] = true;
      }
    }
    sort(initialColumns.begin(), initialColumns.end());
    for(int i = 0; i < (int)initialColumns.size(); i++){
      nodeStack.push(&_graph[initialColumns[i]]);
    }
    int time = 0;
    while(!nodeStack.empty()){
      column *c = nodeStack.top();
      nodeStack.pop();
      if(c->visited){
	finishingTimes[c->index] = time++;
	continue;
      }
      c->visited = true;
      nodeStack.push(c);
      
      vector<int> neighbors;
      for(int i = 0; i < (int)c->colSeqPos.size(); i++){
	seqPos sp = c->colSeqPos[i];
	sp.second++;
	if(sp.second >= (int)_seqs[sp.first].size())
	  continue;
	if(!inStack[_seqPosMap[sp]]){
	  neighbors.push_back(_seqPosMap[sp]);
	  inStack[_seqPosMap[sp]] = true;
	}
      }
      sort(neighbors.begin(), neighbors.end());
      for(int i = 0; i < (int)neighbors.size(); i++){
	nodeStack.push(&_graph[neighbors[i]]);
      }
    }
    
    vector<column> tempGraph;
    for(int i = 0; i < (int)_graph.size(); i++){
      if(_graph[i].isDead){
	continue;
      }
      if(finishingTimes[i] < 0)
	cerr << "DEU PAU TIMES" << endl;
      _graph[i].index = finishingTimes[i]; 
      tempGraph.push_back(_graph[i]);
    }
    sort(tempGraph.begin(), tempGraph.end(), biggerIndex);
    for(int i = 0; i < _numberOfColumns; i++){
      tempGraph[i].index = i;
      _graph[i] = tempGraph[i];
    }
    _graph.erase(_graph.begin()+_numberOfColumns,_graph.end());
  }
      
  void MultipleAlignment::unvisit(vector<column> &v){
    for(int i = 0; i < (int)v.size(); i++){
      _graph[v[i].index].visited = false;
    }
  }

  void MultipleAlignment::addToGappedGraph(postProb p){
    if(p.pos1.second == (int)_seqs[p.pos1.first].size() && p.pos2.second == (int)_seqs[p.pos2.first].size()){
      cout << "Tried to add gap-gap pair to the graph" << endl;
      exit(-1);
    }
    if(p.pos1.second == (int)_seqs[p.pos1.first].size()){
      int index = _seqPosMap[p.pos2];
      for(int i = 0; i < (int)_graph[index].colSeqPos.size(); i++){
	if(p.pos1.first == _graph[index].colSeqPos[i].first)
	  return;
      }
      _graph[index].colSeqPos.push_back(p.pos1);
      return;
    }
    if(p.pos2.second == (int)_seqs[p.pos2.first].size()){
      int index = _seqPosMap[p.pos1];
      for(int i = 0; i < (int)_graph[index].colSeqPos.size(); i++){
	if(p.pos2.first == _graph[index].colSeqPos[i].first)
	  return;
      }
      _graph[index].colSeqPos.push_back(p.pos2);
      return;
    }
    
    int index1 = _seqPosMap[p.pos1];
    int index2 = _seqPosMap[p.pos2];
    int ub,lb;
    seqPos ubsp, lbsp;
    if(index1 > index2){
      ub = index1;
      ubsp = p.pos1;
      lb = index2;
      lbsp = p.pos2;
    }
    else if(index2 > index1){
      ub = index2;
      ubsp = p.pos2;
      lb = index1;
      lbsp = p.pos1;
    }
    else
      return;

    for(int i = 0; i < (int)_graph[ub].colSeqPos.size(); i++){
      for(int j = 0; j < (int)_graph[lb].colSeqPos.size(); j++){
	if(_graph[ub].colSeqPos[i].first == _graph[lb].colSeqPos[i].first && _graph[ub].colSeqPos[i].second != _graph[lb].colSeqPos[i].second)
	  return;
      }
    }

    vector<column> df,db;
    _graph[ub].colSeqPos.push_back(lbsp);
    bool cycle = dfsf(ub,_graph[lb],df);
    unvisit(df);
    if(cycle){
      _graph[ub].colSeqPos.pop_back();
      return;
    }
    dfsb(lb,_graph[ub],db);
    unvisit(db);
    if(db[0].index != ub)
      cerr << "DEU PAUU!!" << endl;
    if(df[0].index != lb)
      cerr << "DEU PAUU!!" << endl;
    assert(db[0].index == ub);
    assert(df[0].index == lb);
    merge(&db[0],&df[0]);
    reorder(df,db);    
  }


  //////////////////////////////////////////////////
  //////Debug functions/////////////////////////////
  //////////////////////////////////////////////////

  void MultipleAlignment::printPPList(){
    if(_postProbs.empty())
      return;
    list<postProb>::iterator it = _postProbs.begin();
    while(it != _postProbs.end()){
      cout << "(" << (*it).prob << "," << (*it).pos1.first << "," << (*it).pos1.second << "," << (*it).pos2.first << "," << (*it).pos2.second << ") ";
      it++;
    }
    cout << endl;
  }
}
