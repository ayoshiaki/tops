/*
 *       DegenerateDistribution.hpp
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

#ifndef DEGENERATE_DISTRIBUTION_H
#define DEGENERATE_DISTRIBUTION_H

#include "crossplatform.hpp"

#include "DiscreteIIDModel.hpp"

namespace tops {
  //! A probabilistic model that emits a constant integer value.
  class DLLEXPORT DegenerateDistribution : public DiscreteIIDModel
  {
    friend class boost::serialization::access;
    template <class Archive>
    void serialize (Archive & ar, const unsigned int )
    {
      ar & boost::serialization::base_object<DiscreteIIDModel> (*this);
      ar & _constant;
    }
  private:
    int _constant;
    double _huge_;
    double _zero;
  public:
    ~DegenerateDistribution() {}
    DegenerateDistribution() {
      _huge_ = -HUGE;
      _zero = 0.0;
    }
    DegenerateDistribution(int c) : DiscreteIIDModel("DegenerateDistribution")
    {
      _huge_ = -HUGE;
      _zero = 0.0;
      _constant=c;
    }

    //! Calculates the sequence likelihood given the model
    virtual double evaluate(const Sequence & sequence, unsigned int begin, unsigned int end) const
    {
      for (int i = begin; (i < (int)sequence.size()) && (i < (int)end);  i++)
        if (sequence[i] != _constant)
          return -HUGE;
      return 0.0;
    }

    //! Generates a sequence by the simulation of the model
    virtual double choose(Sequence & s, unsigned int size) const
    {
      s.resize(size);
      for (int i = 0; i < (int)size; i++)
        s[i] = _constant;
      return 0.0;
    }
    virtual std::string str() const
    {
          std::stringstream out;
          out << _constant << std::endl;
          return out.str();
    }
    virtual double & log_probability_of(int c)
    {
      if(c != _constant)
        return _huge_;
      return _zero;
    }

    virtual int choose () const
    {
      return _constant;
    }
  };
  typedef boost::shared_ptr <DegenerateDistribution> DegenerateDistributionPtr;
}

#endif
