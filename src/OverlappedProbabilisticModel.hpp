/*
 *       OverlappedProbabilisticModel.hpp
  *
 *       Copyright 2011 Waldir Caro <waldirc@ime.usp.br>
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


#ifndef OVERLAP_MODEL_HPP
#define OVERLAP_MODEL_HPP

#include "crossplatform.hpp"

#include "ProbabilisticModelDecorator.hpp"
#include "NeuralNetworkModel.hpp"
#include "Symbol.hpp"
namespace tops {

  /**
   * @class OverlappedProbabilisticModel
   * @brief A decorator class for probabilistic models that handles overlapping sequences.
   *
   * The OverlappedProbabilisticModel class extends the functionality of a base probabilistic model
   * by incorporating overlapping sequences. This class is designed to work with sequences that have
   * overlapping regions, allowing for more complex probabilistic evaluations and operations.
   *
   * @note This class inherits from ProbabilisticModelDecorator.
   *
   * @details
   * The class provides methods to evaluate the probabilistic model over a given sequence within a
   * specified range, choose sequences based on certain criteria, compute prefix sum arrays, and
   * initialize these arrays. It also includes methods to retrieve the model's parameters and name,
   * and to initialize the model with specific overlap and virtual size parameters.
   *
   * @var int OverlappedProbabilisticModel::_seqLength
   * The length of the sequence.
   *
   * @var int OverlappedProbabilisticModel::_left_overlap
   * The size of the left overlap.
   *
   * @var int OverlappedProbabilisticModel::_right_overlap
   * The size of the right overlap.
   *
   * @var int OverlappedProbabilisticModel::_model_virtual_size
   * The virtual size of the model.
   *
   * @var AlphabetPtr OverlappedProbabilisticModel::revAlphabet
   * A pointer to the reverse alphabet.
   *
   * @var DoubleVector OverlappedProbabilisticModel::_overlapped_sum_array
   * A vector to store the overlapped sum array.
   *
   * @fn OverlappedProbabilisticModel::OverlappedProbabilisticModel(ProbabilisticModelPtr m)
   * @brief Constructor that initializes the model with a given probabilistic model.
   * @param m A pointer to the base probabilistic model.
   *
   * @fn OverlappedProbabilisticModel::~OverlappedProbabilisticModel()
   * @brief Destructor for the OverlappedProbabilisticModel class.
   *
   * @fn double OverlappedProbabilisticModel::evaluate(const Sequence & s, unsigned int begin, unsigned int end) const
   * @brief Evaluates the probabilistic model over a given sequence within a specified range.
   * @param s The sequence to be evaluated.
   * @param begin The starting position of the range within the sequence.
   * @param end The ending position of the range within the sequence.
   * @return The evaluation result as a double.
   *
   * @fn Sequence & OverlappedProbabilisticModel::choose(Sequence & h, int size) const
   * @brief Chooses a sequence based on the given size.
   * @param h The sequence to be chosen.
   * @param size The size criteria for choosing the sequence.
   * @return The chosen sequence.
   *
   * @fn Sequence & OverlappedProbabilisticModel::choose(Sequence & h, int initial_phase, int size) const
   * @brief Chooses a sequence based on the given initial phase and size.
   * @param h The sequence to be chosen.
   * @param initial_phase The initial phase criteria for choosing the sequence.
   * @param size The size criteria for choosing the sequence.
   * @return The chosen sequence.
   *
   * @fn double OverlappedProbabilisticModel::prefix_sum_array_compute(int begin, int end)
   * @brief Computes the prefix sum array for the given range.
   * @param begin The starting position of the range.
   * @param end The ending position of the range.
   * @return The computed prefix sum as a double.
   *
   * @fn double OverlappedProbabilisticModel::prefix_sum_array_compute(int begin, int end, int phase)
   * @brief Computes the prefix sum array for the given range and phase.
   * @param begin The starting position of the range.
   * @param end The ending position of the range.
   * @param phase The phase for computing the prefix sum.
   * @return The computed prefix sum as a double.
   *
   * @fn bool OverlappedProbabilisticModel::initialize_prefix_sum_array(const Sequence & s, int phase)
   * @brief Initializes the prefix sum array for the given sequence and phase.
   * @param s The sequence for which the prefix sum array is to be initialized.
   * @param phase The phase for initializing the prefix sum array.
   * @return True if initialization is successful, false otherwise.
   *
   * @fn bool OverlappedProbabilisticModel::initialize_prefix_sum_array(const Sequence & s)
   * @brief Initializes the prefix sum array for the given sequence.
   * @param s The sequence for which the prefix sum array is to be initialized.
   * @return True if initialization is successful, false otherwise.
   *
   * @fn std::string OverlappedProbabilisticModel::str() const
   * @brief Returns a string representation of the model.
   * @return The string representation of the model.
   *
   * @fn std::string OverlappedProbabilisticModel::model_name() const
   * @brief Returns the name of the model.
   * @return The name of the model.
   *
   * @fn ProbabilisticModelParameters OverlappedProbabilisticModel::parameters() const
   * @brief Returns the parameters of the model.
   * @return The parameters of the model.
   *
   * @fn void OverlappedProbabilisticModel::initialize(int left_overlap, int right_overlap, int model_virtual_size)
   * @brief Initializes the model with specific overlap and virtual size parameters.
   * @param left_overlap The size of the left overlap.
   * @param right_overlap The size of the right overlap.
   * @param model_virtual_size The virtual size of the model.
   */
  class DLLEXPORT OverlappedProbabilisticModel : public ProbabilisticModelDecorator {
  private:
    
    int _seqLength;
    int _left_overlap;
    int _right_overlap;
    int _model_virtual_size;
    int _reverse;

    bool _initialized;
    
    AlphabetPtr revAlphabet;
    DoubleVector _overlapped_sum_array;
  public:
    OverlappedProbabilisticModel(ProbabilisticModelPtr m) : ProbabilisticModelDecorator(m)
    {      
    }
    /*OverlappedProbabilisticModel() : ProbabilisticModelDecorator()
    {      
    }*/ 
      
    virtual ~OverlappedProbabilisticModel(){};    
    virtual double evaluate(const Sequence & s, unsigned int begin, unsigned int end) const;
    virtual Sequence & choose(Sequence & h, int size) const ;
    virtual Sequence & choose(Sequence &h, int initial_phase, int size) const;
    virtual double prefix_sum_array_compute(int begin , int end) ;
    virtual double prefix_sum_array_compute(int begin , int end, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s, int phase);
    virtual bool initialize_prefix_sum_array(const Sequence & s) ;
    virtual std::string str() const;
    std::string model_name () const;
    virtual ProbabilisticModelParameters parameters() const ;
    virtual void initialize(int left_overlap, int right_overlap, int model_virtual_size, int reverse) ;

  };
  typedef boost::shared_ptr<OverlappedProbabilisticModel> OverlappedProbabilisticModelPtr;
}

#endif