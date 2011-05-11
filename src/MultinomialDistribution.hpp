/*
 *       MultinomialDistribution.hpp
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

#ifndef FINITE_DISCRETE_DISTRIBUTION_H
#define FINITE_DISCRETE_DISTRIBUTION_H

#include "crossplatform.hpp"

#include "Sequence.hpp"
#include "Alphabet.hpp"

#include "ProbabilisticModel.hpp"
#include "FactorableModel.hpp"
#include "util.hpp"
#include <cstdarg>
#include <vector>

namespace tops {

  //! This  represent  probability distributions  over a finite set of symbols
  class DLLEXPORT MultinomialDistribution :  public FactorableModel
  {
  private:
    DoubleVector _log_probabilities;
    Matrix _log_probabilities_matrix;
    double _huge_;
    int _size;
    bool _geometric_tail;
    double _mean;
  public:

    MultinomialDistribution()  {
      _geometric_tail = false;
    };
    //! Constructor
    /*! \param probabilities is the probabilities value */
    MultinomialDistribution(const DoubleVector & probabilities) ;



    MultinomialDistribution(const Matrix & probabilities);

    //! Choose
    virtual double choose()const ;

    virtual void choosePair(int* a, int* b)const;

    //! Returns the log_probability_of the number s
    virtual double log_probability_of(int s) const;

    virtual double log_probability_of_pair(int s1, int s2) const;

    void strMatrix () const;

    //! Set the probability value of the number s
    virtual double log_probability_of(int s, double new_value) ;


    virtual double evaluatePosition(const Sequence & s, unsigned int i) const ;

    virtual double log_probability_of_pair(int s1, int s2, double new_value);

    virtual double choosePosition(const Sequence & s, int i )const ;

    virtual std::string model_name() const {
      return "MultinomialDistribution";
    }

    virtual ProbabilisticModelCreatorPtr getFactory () const;

    virtual int size() const;
    virtual std::string str() const;

    virtual void initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet) ;

    virtual void initialize(const ProbabilisticModelParameters & p) ;

    virtual ProbabilisticModelParameters parameters() const;

    void setProbabilities(const DoubleVector & probabilities) ;
  };

  typedef boost::shared_ptr<MultinomialDistribution> MultinomialDistributionPtr;
}


#endif
