#include "InhomogeneousFactorableModel.hpp"
#include "Symbol.hpp"
namespace tops {
  double InhomogeneousFactorableModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
  {
    double result = 0.0;
    int t = 0;
    int maximumTime = maximumTimeValue();
    for (int i = (int) begin; (i <= (int) end); i++) {
      if (((i - (int) begin) > maximumTime)
	  && (!isPhased()))
	break;
      result += evaluatePosition(s, i, t);
      t = mod(t + 1, maximumTime + 1);
    }
    return result;
  }

  double InhomogeneousFactorableModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end, int phase) const
  {
    double result = 0.0;
    int t = phase;
    int maximumTime = maximumTimeValue();
    for (int i = (int) begin; (i <= (int) end); i++) {
      if (((i - (int) begin) > maximumTime)
	  && (!isPhased()))
	break;
      result += evaluatePosition(s, i, t);
      t = mod(t + 1, maximumTime + 1);
    }
    return result;
  }


  Sequence & InhomogeneousFactorableModel::choose(Sequence & h, int size) const
  {
    chooseWithHistory(h,0,size);
    return h;
  }

  Sequence & InhomogeneousFactorableModel::chooseWithHistory(Sequence & h, int i, int size) const
  {
    chooseWithHistory(h,i,0,size);
    return h;
  }

  double InhomogeneousFactorableModel::prefix_sum_array_compute(int begin, int end, int phase) {
    if (isPhased()) {
      int nphase = maximumTimeValue()+1;
      int p = mod(begin, nphase);
      int k = 0;
      while ((phase >= 0) && (mod(p + k, nphase) != phase))
	k++;
      p = k;

      if ((end + 1) >= (int) _precision[p].size())
	end = _precision[p].size() - 2;
      double t = _precision[p][end + 1] - _precision[p][begin];
      if ((t > 0) || (begin > end))
	return -HUGE;
      if(_alpha[p][end + 1] - _alpha[p][begin] > 0) 
	{
	  std::cerr << "ERROR: InhomogeneousFactorableModel" << _alpha[p][end + 1] << " " <<  _alpha[p][begin] << " " << t << std::endl;
	}
      assert(_alpha[p][end + 1] - _alpha[p][begin] <= 0);
      
      return _alpha[p][end + 1] - _alpha[p][begin];
    } else {
      if ((begin < (int) _scores.size()) && (begin >= 0))
	return _scores[begin];
      return -HUGE;
    }
    
  }
  double InhomogeneousFactorableModel::prefix_sum_array_compute(int begin, int end) 
  {
    return prefix_sum_array_compute(begin, end, 0);
  }
    
  bool InhomogeneousFactorableModel::initialize_prefix_sum_array(const Sequence & s, int phase)
  {
    if(ProbabilisticModel::initialize_prefix_sum_array(s))
      return true;
    
    if (isPhased())
      {

	
	_phase = phase;
	int nphases = maximumTimeValue()+1;
	_alpha.resize(nphases);
	_precision.resize(nphases);
	for (int k = 0; k < nphases; k++) {
	  _alpha[k].resize(s.size() + 1);
	  _precision[k].resize(s.size() + 1);
	}
	for (int k = 0; k < nphases; k++) {
	  _alpha[k][0] = 0;
	  _precision[k][0] = 0;
	  for (int i = 0; i < (int) s.size(); i++) {
	    int p = mod(i + k, nphases);
	    double prob = evaluatePosition(s, i, p);
	    if ((prob != prob) || close(prob, -HUGE, 1e-10)) {
	      _precision[k][i + 1] = _precision[k][i] + 1;
	      _alpha[k][i + 1] = 0;
	    } else {
	      _precision[k][i + 1] = _precision[k][i];
	      _alpha[k][i + 1] = _alpha[k][i] + prob;
	    }
#if 0
	    std::cerr << i << " " << alphabet()->getSymbol(s[i])->name() << " " << k << " " << p << " " << prob << " " << _alpha[k][i] << " " << _precision[k][i] << " " << std::endl;
#endif
	  }

	}

      }
    else {
      _scores.resize(s.size());
      for (int i = 0; i < (int) s.size(); i++) 
	_scores[i] = evaluate(s, i, s.size() - 1);
    }
    return true; 
  }

  Sequence & InhomogeneousFactorableModel::chooseWithHistory(Sequence & h, int i,  int initial_phase, int size) const{
    int maximumTime = maximumTimeValue();
    int t = initial_phase;
    if(t < 0) t = 0;
    if(isPhased())
      t = mod(initial_phase, maximumTime+1);
    if((size  > maximumTime) && !isPhased())
      size = maximumTime+1;
    
    for(int k = i; (k < (size+i)); k++)
      {
	if( (t >  maximumTime) && (!isPhased()))
	  break;
	if(k < (int)h.size())
	  h[k] = choosePosition(h, k, t);
	else
	  h.push_back(choosePosition(h, k,t));
	t = mod(t+1, maximumTime+1);
      }
    return h;

  }
}
