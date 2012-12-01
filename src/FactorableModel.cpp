/*
 *       FactorableModel.cpp
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

#include "FactorableModel.hpp"

namespace tops {
    double FactorableModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
    {
        double result = 0.0;
        if (end >= s.size())
            return -HUGE;
        for(unsigned int i = begin; i <= end; i++)
            {
                result += evaluatePosition(s, i);
            }
        return result;
    }

    Sequence & FactorableModel::choose(Sequence & h, int size) const
    {
        chooseWithHistory(h,0,size);
        return h;
    }


    Sequence & FactorableModel::chooseWithHistory(Sequence & h, int i,  int size) const
    {
        for (int k = i; k < (size+i); k++)
            {
                if (k < (int)h.size())
                    h[k] = choosePosition(h, k);
                else
                    h.push_back(choosePosition(h, k));
            }
        return h;
    }

    double FactorableModel::prefix_sum_array_compute(int begin, int end)
    {
        if(begin>end)
            return -HUGE;

        if((begin >= 0) && ((end + 1) < (int) _alpha.size()))
            {
                if((_precision[end+1] - _precision[begin]) > 0)
                    return -HUGE;
                return _alpha[end+1] - _alpha[begin];
            }
        else
            return -HUGE;

    }

    bool FactorableModel::initialize_prefix_sum_array(const Sequence & s)
    {
        if(ProbabilisticModel::initialize_prefix_sum_array(s))
            {
                return true;
            }
        _alpha.resize(s.size() + 1);
        _precision.resize(s.size() + 1);
        _alpha[0] = 0;
        for(int i = 0 ; i < (int) s.size() ; i++) {
            double prob = evaluate(s, i, i);
            if(close(prob, -HUGE, 1e-1))
                {
                    _precision[i+1] = _precision[i]+1;
                    _alpha[i+1] = evaluate(s,i,i);
                }
            else
                {
                    _alpha[i+1] = _alpha[i] +   prob;
                    _precision[i+1] = _precision[i];
                }
        }
#if 0
        for(int i = 0; i < s.size(); i++)
            std::cerr << " "  << i << " " <<  _alpha[i] << " " << _precision[i] << std::endl;
#endif
        return true;
    }


}
