/*
 * profileHMMtests.cpp
 *
 *  Created on: 17/05/2012
 *      Author: felipe
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>

#include "ProfileHiddenMarkovModel.hpp"
#include "Alphabet.hpp"
#include "Symbol.hpp"
#include "Sequence.hpp"
#include "SequenceFactory.hpp"

using namespace tops;
using namespace std;

#define MATCH   0
#define INS     1
#define DEL     2
#define SYM     3
#define GAP     4

/* n_states sendo numero de match states incluindo start e end vezes 3 */
int state_ind (int n_states, int type, int index) {
  assert (n_states % 3 == 0);
  int n = n_states/3;
  return type*n + index;
}

int get_insertion_cols (vector <int> & res, SequenceList seqs, SymbolPtr gap) {
        SequenceList::iterator si;
        vector < int >::iterator c;
        int i, j, count, nseqs, seqslen, alptlen;

        if (seqs.size() <= 0) return 0;

        nseqs = (int)seqs.size();
        seqslen = (int)seqs.begin()->size();

        for (i = 0; i < seqslen; ++i) {
                count = 0;
                cout << "posicao " << i << endl;
                for (si = seqs.begin(); si != seqs.end(); ++si) {

                    if ((*si)[i] == gap->id()) {
                        count++;
                        if (count > nseqs/2) {
                            break;
                        }
                    }
                    cout << "count = " << count << endl;
                }

                if (si != seqs.end()) {
                    res.push_back(1);
                }
                else {
                    res.push_back(0);
                }
        }

        for (i = 0; i < (int)res.size(); ++i) {
                cout << "res[" << i << "]= " << res[i] << endl;
        }

        return 1;
}

Sequence sequence_from_string (SequenceFactory f, string seq) {
    stringstream s;
    for (int i = 0; i < seq.size(); ++i) {
        if(i == 0) { s << seq[i];        }
        else 	   { s << " " << seq[i]; }
    }

    return f.createSequence(s.str());
}

void printEmissionMatrix(Matrix & emiss, vector <int> states, AlphabetPtr alphabet) {
  int n_match = states.size()/3;
  int n_ins = n_match;

  cout << "Match emissions:" << endl;
  for (int i = 0; i < alphabet->size(); ++i) {
      for (int j = 0; j < n_match - 1; ++j) {
          printf("%.5lf\t", emiss (states[ state_ind (states.size(), MATCH, j)], i));
      }
      cout << endl;
  }
  cout << endl;

  cout << "Insert emissions:" << endl;
  for (int i = 0; i < alphabet->size(); ++i) {
      for (int j = 0; j < n_ins - 1; ++j) {
          printf("%.5lf\t", emiss (states[ state_ind (states.size(), INS, j)], i));
      }
      cout << endl;
  }
  cout << endl;
}

void printTransitionMatrix (Matrix & trans, vector <int> states, AlphabetPtr alphabet) {

  int n_match = states.size()/3;
  cout << "Transition Matrix:" << endl;
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
          for (int col = 0; col < n_match - 1; ++col) {
              if (j == INS) {
                  printf("%.5lf\t", trans (states[state_ind(states.size(), i, col)], states[state_ind(states.size(), j, col)]));
              }
              else {
                  printf("%.5lf\t", trans (states[state_ind(states.size(), i, col)], states[state_ind(states.size(), j, col + 1)]));
              }
          }
          cout << endl;
      }
      cout << endl;
  }
}

int get_case (int col, int cel) {
  if (col == MATCH && cel == SYM) { return MATCH; }
  if (col == MATCH && cel == GAP) { return DEL; }
  if (col == INS && cel == SYM) { return INS; }
  return -1;
}

std::string case_string(int x) {
  switch(x) {
  case MATCH: return "MATCH";
  case INS: return "INS";
  case DEL: return "DEL";
  case SYM: return "SYM";
  case GAP: return "GAP";
  }
  return "INVALID";
}

int main(int argc, char ** argv) {
  AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
  alphabet->createSymbol("A");
  alphabet->createSymbol("C");
  alphabet->createSymbol("G");
  alphabet->createSymbol("T");
  SymbolPtr gap = alphabet->createSymbol("-");

  SequenceFactory f(alphabet);

  SequenceList seqs;
  /*
  seqs.push_back(sequence_from_string(f, "ACGCGT-ATC---"));
  seqs.push_back(sequence_from_string(f, "A-GCGT-ATC-A-"));
  seqs.push_back(sequence_from_string(f, "A-GCGT-ATC-T-"));
  seqs.push_back(sequence_from_string(f, "-CGCGT-ATC-G-"));
  seqs.push_back(sequence_from_string(f, "--GCGT-ATC-AG"));
  seqs.push_back(sequence_from_string(f, "GCGCGGGA-C---"));
  seqs.push_back(sequence_from_string(f, "A-GGGT--TCGA-"));
  */
  seqs.push_back(sequence_from_string(f, "AG---C"));
  seqs.push_back(sequence_from_string(f, "A-AG-C"));
  seqs.push_back(sequence_from_string(f, "AG-AA-"));
  seqs.push_back(sequence_from_string(f, "--AAAC"));
  seqs.push_back(sequence_from_string(f, "AG---C"));

  int n_seqs = seqs.size();
  int align_size = seqs.begin()->size();

  vector<int>::iterator x;
  for (x = seqs.front().begin(); x != seqs.front().end(); ++x) {
          cout << (*x) << endl;
  }

  vector < int > is_ins;
  //get_insertion_cols(is_ins, seqs, gap);
  is_ins.push_back(0);
  is_ins.push_back(0);
  is_ins.push_back(1);
  is_ins.push_back(1);
  is_ins.push_back(1);
  is_ins.push_back(0);

  int n_match_states = 2;
  for (vector<int>::iterator i = is_ins.begin(); i != is_ins.end(); ++i) {
      if (*i == 0) {
          n_match_states++;
      }
  }
  cout << "Numero de match states = " << n_match_states << endl;
  int n_states = n_match_states*3;

  vector <int> states;
  states.resize(n_states, 0);
  for (int i = 0; i < states.size(); ++i) {
      states[i] = i;
  }

  Matrix emiss(n_states, alphabet->size());
  Matrix trans(n_states, n_states);


  // Initializing matrices with zero
  for (int i = 0; i < emiss.size1(); ++i) {
    for (int j = 0; j < emiss.size2(); ++j) {
        emiss(i,j) = 0;
    }
  }
  for (int i = 0; i < trans.size1(); ++i) {
    for (int j = 0; j < trans.size2(); ++j) {
        trans(i,j) = 0;
    }
  }


  // Counting emissions
  int pos_model = 0;
  for (int pos_align = 0; pos_align < align_size; ++pos_align) {
      if (is_ins[pos_align]) {
          for (int j = 0; j < seqs.size(); ++j) {
              if (seqs[j][pos_align] != gap->id()) {
                  emiss (state_ind (n_states, INS, pos_model), seqs[j][pos_align])++;
              }
          }
      }
      else {
          pos_model++;
          for (int j = 0; j < n_seqs; ++j) {
              if (seqs[j][pos_align] != gap->id()) {
                  emiss (state_ind (n_states, MATCH, pos_model), seqs[j][pos_align])++;
              }
          }
      }
  }
  printEmissionMatrix(emiss,states,alphabet);


  // Counting transitions
  for (int i = 0; i < n_seqs; ++i) {
      int model_pos = 0;
      int prev = MATCH;

      //cout << "Sequencia " << i << endl;
      for (int j = 0; j < align_size; ++j) {
          int curr_cel = (seqs[i][j] == gap->id())? GAP:SYM;
          int curr_col = (is_ins[j])? INS:MATCH;
          int curr = get_case (curr_col, curr_cel);

          //cout << "Prev = " << case_string(prev)  << " Curr = " << case_string(curr) <<endl;
          if (curr != -1) {
              //cout << case_string(curr) << " " << model_pos << endl;

              if (curr == INS) {
                  trans (state_ind(n_states, prev, model_pos), state_ind(n_states, curr, model_pos))++;
              }
              else {
                  trans (state_ind(n_states, prev, model_pos), state_ind(n_states, curr, model_pos + 1))++;
              }
              prev = curr;
              if (!is_ins[j]) {
                  model_pos++;
              }
          }
      }
      trans (state_ind(n_states, prev, model_pos), state_ind(n_states, MATCH, model_pos + 1))++;
  }
  printTransitionMatrix(trans,states,alphabet);

  //Emission Pseudocounts
  for (int i = 0; i < n_match_states - 1; ++i) {
      for (int symbol = 0; symbol < alphabet->size(); ++symbol) {
          if (i != 0)
            emiss (state_ind(n_states, MATCH, i), symbol)++;
          emiss (state_ind(n_states, INS, i), symbol)++;
      }
  }
  printEmissionMatrix(emiss,states,alphabet);

  //Transitions Pseudocounts
  for (int i = 0; i < n_match_states - 1; ++i) {
      trans (state_ind(n_states, MATCH, i), state_ind(n_states, MATCH, i + 1))++;
      trans (state_ind(n_states, MATCH, i), state_ind(n_states, INS, i))++;
      trans (state_ind(n_states, INS, i), state_ind(n_states, MATCH, i + 1))++;
      trans (state_ind(n_states, INS, i), state_ind(n_states, INS, i))++;

      if (i != 0) {
        trans (state_ind(n_states, DEL, i), state_ind(n_states, MATCH, i + 1))++;
        trans (state_ind(n_states, DEL, i), state_ind(n_states, INS, i))++;
        if (i != n_match_states - 2) {
            trans (state_ind(n_states, DEL, i), state_ind(n_states, DEL, i + 1))++;
        }
      }
      if (i != n_match_states - 2) {
          trans (state_ind(n_states, MATCH, i), state_ind(n_states, DEL, i + 1))++;
          trans (state_ind(n_states, INS, i), state_ind(n_states, DEL, i + 1))++;
      }
  }
  printTransitionMatrix(trans,states,alphabet);

  //Emission probabilities
  for (int i = 1; i < n_match_states - 1; ++i) {
      int sum = 0;
      for (int symbol = 0; symbol < alphabet->size(); ++symbol) {
          sum += emiss (state_ind(n_states, MATCH, i), symbol);
      }
      for (int symbol = 0; symbol < alphabet->size(); ++symbol) {
          emiss (state_ind(n_states, MATCH, i), symbol) /= sum;
      }
  }
  for (int i = 0; i < n_match_states - 1; ++i) {
        int sum = 0;
        for (int symbol = 0; symbol < alphabet->size(); ++symbol) {
            sum += emiss (state_ind(n_states, INS, i), symbol);
        }
        for (int symbol = 0; symbol < alphabet->size(); ++symbol) {
            emiss (state_ind(n_states, INS, i), symbol) /= sum;
        }
  }
  printEmissionMatrix(emiss,states,alphabet);

  //Transition probabilities
  for (int i = 0; i < n_match_states - 1; ++i) {
      int sum = 0;
      sum += trans (state_ind(n_states, MATCH, i), state_ind(n_states, MATCH, i + 1));
      sum += trans (state_ind(n_states, MATCH, i), state_ind(n_states, INS, i));
      if (i != n_match_states - 2) {
        sum += trans (state_ind(n_states, MATCH, i), state_ind(n_states, DEL, i + 1));
        trans (state_ind(n_states, MATCH, i), state_ind(n_states, DEL, i + 1)) /= sum;
      }
      trans (state_ind(n_states, MATCH, i), state_ind(n_states, MATCH, i + 1)) /= sum;
      trans (state_ind(n_states, MATCH, i), state_ind(n_states, INS, i)) /= sum;
  }
  for (int i = 0; i < n_match_states - 1; ++i) {
      int sum = 0;
      sum += trans (state_ind(n_states, INS, i), state_ind(n_states, MATCH, i + 1));
      sum += trans (state_ind(n_states, INS, i), state_ind(n_states, INS, i));
      if (i != n_match_states - 2) {
        sum += trans (state_ind(n_states, INS, i), state_ind(n_states, DEL, i + 1));
        trans (state_ind(n_states, INS, i), state_ind(n_states, DEL, i + 1)) /= sum;
      }
      trans (state_ind(n_states, INS, i), state_ind(n_states, MATCH, i + 1)) /= sum;
      trans (state_ind(n_states, INS, i), state_ind(n_states, INS, i)) /= sum;
  }
  for (int i = 1; i < n_match_states - 1; ++i) {
      int sum = 0;
      sum += trans (state_ind(n_states, DEL, i), state_ind(n_states, MATCH, i + 1));
      sum += trans (state_ind(n_states, DEL, i), state_ind(n_states, INS, i));
      if (i != n_match_states - 2) {
        sum += trans (state_ind(n_states, DEL, i), state_ind(n_states, DEL, i + 1));
        trans (state_ind(n_states, DEL, i), state_ind(n_states, DEL, i + 1)) /= sum;
      }
      trans (state_ind(n_states, DEL, i), state_ind(n_states, MATCH, i + 1)) /= sum;
      trans (state_ind(n_states, DEL, i), state_ind(n_states, INS, i)) /= sum;
  }
  printTransitionMatrix(trans,states,alphabet);

  return 0;
}


#if 0
        ACGCGT-ATC---
        A-GCGT-ATC-A-
        A-GCGT-ATC-T-
        -CGCGT-ATC-G-
        --GCGT-ATC-AG
        GCGCGGGA-C---
        A-GGGT--TCGA-
#endif
