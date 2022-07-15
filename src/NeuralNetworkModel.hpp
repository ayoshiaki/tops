/*
 *       NeuralNetworkModel.hpp
 *
 *       Copyright 2022 Waldir Caro <waldirc@ime.usp.br>
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

#include <torch/torch.h>

namespace tops {

  //! What kind of model is this? factorable? there is a sequence and an alphabet?
  class DLLEXPORT NeuralNetworkModel : public ProbabilisticModel
  {
  private:
    torch::Tensor _weight;
    torch::Tensor _bias;
  public:
    
    NeuralNetworkModel() ;
    //! Constructor
    /*! \param bias is the bias value of the network
        \param weight is the weight value of the network
     */
    NeuralNetworkModel(const torch::Tensor weight, const torch::Tensor bias) ;



    //! Choose
    virtual double choose()const ;
    virtual Sequence & choose(Sequence & s, int size ) const ;

    //virtual void choosePair(int* a, int* b) const;

    //! Returns the log_probability_of the number s
    //virtual double log_probability_of(int s) const;

    //virtual double log_probability_of_pair(int s1, int s2) const;

    //void strMatrix () const;

    //! Set the probability value of the number s
    //virtual double log_probability_of(int s, double new_value) ;

    //virtual double evaluatePosition(const Sequence & s, unsigned int i) const ;

    //virtual double log_probability_of_pair(int s1, int s2, double new_value);

    //virtual double choosePosition(const Sequence & s, int i )const ;

    
    
    virtual std::string model_name() const {
      return "Neural Network Model";
    }

    virtual ProbabilisticModelCreatorPtr getFactory () const;

    //virtual int size() const;
    virtual std::string str() const;

    //virtual void initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet) ;

    virtual void initialize(const ProbabilisticModelParameters & p) ;

    virtual ProbabilisticModelParameters parameters() const;

    void setParameters(const torch::Tensor weight, const torch::Tensor bias) ;
    
  };

  typedef boost::shared_ptr<NeuralNetworkModel> NeuralNetworkModelPtr;
}


#endif
