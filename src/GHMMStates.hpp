/*
 *       GHMMStates.hpp
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

#ifndef GHMM_STATES_HPP
#define GHMM_STATES_HPP

#include "crossplatform.hpp"
#include "util.hpp"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <string>
#include <list>
#include "ProbabilisticModelParameter.hpp"


namespace tops {

  class DLLEXPORT ProbabilisticModel;
  typedef boost::shared_ptr <ProbabilisticModel> ProbabilisticModelPtr;
  class DLLEXPORT DiscreteIIDModel;
  typedef boost::shared_ptr <DiscreteIIDModel> DiscreteIIDModelPtr;
  class DLLEXPORT Symbol;
  typedef boost::shared_ptr<Symbol> SymbolPtr;

    class DLLEXPORT GHMMState;
    typedef boost::shared_ptr<GHMMState> GHMMStatePtr;
    typedef std::vector<GHMMStatePtr> GHMMStates;
  //! Represents a GHMM State
  class DLLEXPORT GHMMState {
  public:
    GHMMState();
    GHMMState(ProbabilisticModelPtr observation,
              DiscreteIIDModelPtr transition, SymbolPtr name) :
      _observation(observation), _transition(transition), _name(name){};
    virtual ~GHMMState();
    virtual void setObservation(ProbabilisticModelPtr obs) ;
    virtual ProbabilisticModelPtr observation() const ;
    virtual void setTransition(DiscreteIIDModelPtr trans) ;
    virtual DiscreteIIDModelPtr transition() const ;
    virtual int chooseDuration() const ;
    virtual std::string name() const ;
    virtual int id() const ;
    virtual void addPredecessor(int id) ;
    virtual std::vector<int> & predecessors() ;
    virtual void addSuccessor(int id) ;
    virtual void clearPredecessorSuccessor() {
      _successors.clear();
      _predecessors.clear();
    }
    virtual std::vector<int> & successors() ;
    virtual std::vector<int> & classes() ;
    virtual void setClasses(std::vector<int> &classes);
    virtual double duration_probability(int l) const ;
    virtual bool isGeometricDuration() const ;
    virtual std::string str() const ;
    virtual int getInputPhase() const ;
    virtual void setInputPhase(int _inputPhase) ;
    virtual int getOutputPhase() const ;
    virtual void setOutputPhase(int _outputPhase) ;

    virtual int getStart() const ;
    virtual void setStart(int start) ;
    virtual int getStop() const ;
    virtual void setStop(int stop) ;

    virtual void isLeftJoinable(int joinable);
    virtual int isLeftJoinable() const;

    virtual void isRightJoinable(int joinable);
    virtual int isRightJoinable() const;


    virtual void observationModelName(std::string name) ;
    virtual void durationModelName(std::string name) ;
    virtual std::string observationModelName() const;
    virtual std::string durationModelName() const;
    virtual void fixTransitionDistribution () const {} ;
    virtual ProbabilisticModelParameters parameters() const;
    virtual void findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions );
    virtual void forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions);
    virtual double backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions);
    virtual void posteriorSum (Matrix & alpha, Matrix &beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber);

    virtual void choosePredecessor (Matrix & alpha, int base, int & state, int & position , const GHMMStates & all_states);

  protected:
    ProbabilisticModelPtr _observation;
    DiscreteIIDModelPtr _transition;
    SymbolPtr _name;
    std::vector<int> _predecessors;
    std::vector<int> _successors;
    std::vector<int> _classes;
    int _inputPhase;
    int _outputPhase;
    int _start;
    int _stop;
    bool _left_joinable;
    bool _right_joinable;
    std::string _observationModelName;
  };
  //! GHMM signal states
  class DLLEXPORT GHMMSignalState: public GHMMState {
  public:
    GHMMSignalState() ;
    GHMMSignalState(ProbabilisticModelPtr observation,
                    DiscreteIIDModelPtr transition, SymbolPtr name) :
      GHMMState(observation, transition, name) {};

    virtual int size() const ;
    virtual void setSize(int s);
    virtual int chooseDuration() const ;
    virtual double getThreshold() const ;
    virtual void setThreshold(double threshold) ;
    virtual double duration_probability(int l) const ;
    virtual std::string str() const ;
    virtual bool isGeometricDuration() const ;

    virtual ProbabilisticModelParameters parameters() const;
    virtual void fixTransitionDistribution () const ;
      virtual void findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions );
      virtual void forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions);
    virtual double backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions);
    virtual void posteriorSum (Matrix & alpha, Matrix &beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber);

      virtual void choosePredecessor (Matrix & alpha, int base, int & state, int & position, const GHMMStates & all_states);

  private:
    int _size;
    double _threshold;

  };
  //! GHMM Explicit duration state
  class DLLEXPORT GHMMExplicitDurationState: public GHMMState {
  public:
    virtual ~GHMMExplicitDurationState() ;

    GHMMExplicitDurationState() ;

    GHMMExplicitDurationState(ProbabilisticModelPtr observation,
                              DiscreteIIDModelPtr transition, SymbolPtr name) :
      GHMMState(observation, transition, name) {};
      virtual void findBestPredecessor (Matrix & gamma, Matrix &psi, IntMatrix &psilen, const Sequence & s, int base, const GHMMStates & all_states, std::map < int, std::list<int> >  & valid_positions );
      virtual void forwardSum (Matrix & alpha, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions);
    virtual double backwardSum(Matrix &beta, const Sequence &s, int base, std::vector< std::list<int> > &valid_positions);
    virtual void posteriorSum (Matrix & alpha, Matrix &beta, fMatrix &postProbs, const Sequence & s, int base, const GHMMStates & all_states, std::vector< std::list<int> > &valid_positions, double prob, int stateNumber);

      virtual void choosePredecessor (Matrix & alpha, int base, int & state, int & position, const GHMMStates & all_states);


    virtual void setDuration(ProbabilisticModelPtr d) ;
    virtual ProbabilisticModelPtr duration() const ;
    virtual int chooseDuration() const ;
    virtual bool isGeometricDuration() const ;
    virtual double duration_probability(int l) const ;
    virtual std::string str() const ;

    virtual void durationModelName(std::string name);
    virtual std::string durationModelName() const ;
    virtual void fixTransitionDistribution () const;

    virtual ProbabilisticModelParameters parameters() const;

  private:
    ProbabilisticModelPtr _duration;
    std::string _durationModelName;
    int _number_of_phases;
  };


  typedef boost::shared_ptr<GHMMSignalState> GHMMSignalStatePtr;
  typedef boost::shared_ptr<GHMMExplicitDurationState>
  GHMMExplicitDurationStatePtr;


  typedef std::vector<GHMMSignalStatePtr> GHMMSignalStates;
  typedef std::vector<GHMMExplicitDurationStatePtr> GHMMExplicitDurationStates;

}

#endif
