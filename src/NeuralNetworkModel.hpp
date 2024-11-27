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
#include <torch/script.h>

namespace tops {

  //! What kind of model is this? factorable? there is a sequence and an alphabet?
  class DLLEXPORT NeuralNetworkModel : public ProbabilisticModel
  {

  private:
    torch::nn::Sequential _module_nn; /*!< Sequential layers to train */    
    torch::jit::script::Module _trained_module_nn; /*!< Layers (not necessarely sequential) that were trained beforehand */    
    std::string _trained_model_file;
    
    int _sequence_length; // it will be the sum of upstream and downstream
    int _upstream_length;
    int _downstream_length;
    
    //! others
    torch::jit::script::Module* non_const_jit_module;
    torch::Tensor _last_sequence;
    DoubleVector _scores;
    bool _initialized = false;
  public:
    
    //! Constructor.
    /*!
      
    */
    NeuralNetworkModel() ;

    //! Constructor.
    /*! 
      \param module_nn_ptr is the pointer to a Sequential architecture of layers
     */
    NeuralNetworkModel(std::shared_ptr<torch::nn::Sequential> module_nn_ptr) ;

    //! Constructor.
    /*! 
      \param trained_model_file file containing a trained neural network (can by of any kind of architecture of layers)
     */
    NeuralNetworkModel(std::string trained_model_file) ;


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

    

    //virtual double log_probability_of_pair(int s1, int s2, double new_value);

    //virtual double choosePosition(const Sequence & s, int i )const ;

    virtual std::string print_graph () const ;
    
    virtual std::string model_name() const {
      return "Neural Network Model";
    }

    virtual ProbabilisticModelCreatorPtr getFactory () const;

    //virtual int size() const;
    virtual std::string str() const;

    //virtual void initializeFromMap(const std::map <std::string, double> & probabilities, AlphabetPtr alphabet) ;

    virtual void initialize(const ProbabilisticModelParameters & p) ;

    virtual ProbabilisticModelParameters parameters() const;

    void setParameters(std::shared_ptr<torch::nn::Sequential> module_nn_ptr, std::string trained_model_file, int upstream_length, int downstream_length) ;

    // Transform a list of sequences into Tensor data
    torch::Tensor sequences_to_Tensor(SequenceList & sample) const;

    // Train the model using the SGD algorithm
    virtual double trainSGDAlgorithm(SequenceList & training_set, int epochs, int batch_size, double learning_rate);

    // Evaluates a Sequence
    // does it make sense to have evaluate for Neural Network class? it is the likelihood of the sequence, perhaps this model does not need it
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end, unsigned int phase) const;
    virtual double evaluatePosition(const Sequence & s, unsigned int i) const ;

    virtual std::vector<int> classify_subsequences_in_batches(const torch::Tensor& stacked_subsequences, int batch_size, double threshold, torch::Device device);
    
    virtual bool initialize_prefix_sum_array(const Sequence & s);
    virtual double prefix_sum_array_compute(int begin , int end);

    DoubleVector getScores() const;
  };

  typedef boost::shared_ptr<NeuralNetworkModel> NeuralNetworkModelPtr;
}


#endif
