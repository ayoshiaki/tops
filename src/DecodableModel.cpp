#include "DecodableModel.hpp"

namespace tops {


  double DecodableModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
  {
    Matrix alpha;
    Sequence s2;
    s2.resize(end - begin + 1);
    for (unsigned int i = begin; i <= end; i++)
      s2[i-begin] = s[i];
    return forward(s2, alpha);
  }
  
  Sequence & DecodableModel::choose(Sequence & h, int size) const
  {
    Sequence p;
    return choose(h,p,0,size);
  }
  
  Sequence & DecodableModel::choose(Sequence & h, Sequence & path,   int size) const{
    return choose(h, path, 0, size);
  }
  Sequence & DecodableModel::choose(Sequence & h, Sequence & path,  int i, int size) const
  {
    assert(path.size() == h.size());
    int state = chooseFirstState();
    if (((i - 1) < (int) path.size()) && (i - 1) >= 0)
      state = path[i - 1];
    int l = 0;
    while ((int) h.size() < (i + size)) {
      chooseObservation(h, i + l, state);
      for (int k = i + l; k < (int) h.size(); k++, l++) {
	if (k < (int) path.size())
	  path[k] = state;
	else
	  path.push_back(state);
      }
      state = chooseState(state); // next state
    }
    h.resize(size);
    path.resize(size);
    return h;
  }
}
