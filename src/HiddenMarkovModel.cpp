/*
 *       HiddenMarkovModel.cpp
 *
 *       Copyright 2011 Andre Yoshiaki Kashiwabara <akashiwabara@usp.br>
 *                      �gor Bon�dio <ibonadio@ime.usp.br>
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

#include <boost/numeric/ublas/matrix.hpp>
#include "Alphabet.hpp"
#include "HiddenMarkovModel.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "Symbol.hpp"
#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdio.h>

namespace tops {
std::string HiddenMarkovModel::getStateName(int state) const {
	return getState(state)->getName()->name();
}

Sequence & HiddenMarkovModel::chooseObservation(Sequence & h, int i, int state) const {
	if ((state >= 0) && (!getState(state)->isSilent()))
		return getState(state)->emission()->chooseWithHistory(h, i, 1);
	return h;
}
int HiddenMarkovModel::chooseState(int state) const {
	return getState(state)->transitions()->choose();
}
int HiddenMarkovModel::chooseFirstState() const {
	return _initial_probability->choose();
}

double HiddenMarkovModel::forward(const Sequence & sequence, Matrix &a) const {
	int nstates = _states.size();
	int size = sequence.size();
	Matrix alpha(nstates, size);
	for (int k = 0; k < nstates; k++)
		alpha(k, 0) = _initial_probability->log_probability_of(k) + getState(k)->emission()->log_probability_of(sequence[0]);

	for (int t = 0; t < size - 1; t++) {
		for (int i = 0; i < nstates; i++) {
			int j = 0;
			if (j < nstates) {
				alpha(i, t + 1) = alpha(j, t) + getState(j)->transitions()->log_probability_of(i);
				for (j = 1; j < nstates; j++)
					alpha(i, t + 1) = log_sum(alpha(i, t + 1), alpha(j, t) + getState(j)->transitions()->log_probability_of(i));
			}
			alpha(i, t + 1) += getState(i)->emission()->log_probability_of(sequence[t + 1]);

		}
	}
	a = alpha;
	double sum = alpha(0, size - 1);
	for (int k = 1; k < nstates; k++)
		sum = log_sum(sum, alpha(k, size - 1));
	return sum;
}

//! Backward algorithm
double HiddenMarkovModel::backward(const Sequence & sequence, Matrix &b) const {
	int nstates = _states.size();
	int size = sequence.size();
	Matrix beta(nstates, size);
	for (int k = 0; k < nstates; k++)
		beta(k, size - 1) = 0.0;
	for (int t = size - 2; t >= 0; t--) {

		for (int i = 0; i < nstates; i++) {
			int j = 0;
			if (j < nstates) {
				beta(i, t) = getState(i)->transitions()->log_probability_of(j) + getState(j)->emission()->log_probability_of(sequence[t + 1]) + beta(j, t + 1);
				for (j = 1; j < nstates; j++)
					beta(i, t) = log_sum(beta(i, t),
							getState(i)->transitions()->log_probability_of(j) + getState(j)->emission()->log_probability_of(sequence[t + 1]) + beta(j, t + 1));
			}
		}
	}
	b = beta;
	double sum = -HUGE;
	for (int k = 0; k < nstates; k++)
		sum = log_sum(sum, beta(k, 0) + _initial_probability->log_probability_of(k) + getState(k)->emission()->log_probability_of(sequence[0]));
	return sum;

}

//! Viterbi algorithm
double HiddenMarkovModel::viterbi(const Sequence &sequence, Sequence &path, Matrix & viterbi) const {
	typedef boost::numeric::ublas::matrix<int> MatrixInt;
	int nstates = _states.size();
	int size = sequence.size();

	Matrix gamma(nstates, size + 1);
	MatrixInt psi(nstates, size + 1);

	for (int k = 0; k < nstates; k++)
		gamma(k, 0) = _initial_probability->log_probability_of(k) + getState(k)->emission()->log_probability_of(sequence[0]);

	for (int i = 0; i < size - 1; i++) {
		for (int k = 0; k < nstates; k++) {
			gamma(k, i + 1) = gamma(0, i) + getState(0)->transitions()->log_probability_of(k);
			psi(k, i + 1) = 0;
			for (int p = 1; p < nstates; p++) { // p is the predecessor
				double v = gamma(p, i) + getState(p)->transitions()->log_probability_of(k); //
				if (gamma(k, i + 1) < v) {
					gamma(k, i + 1) = v;
					psi(k, i + 1) = p;
				}
			}
			gamma(k, i + 1) += getState(k)->emission()->log_probability_of(sequence[i + 1]);
		}
	}

	viterbi = gamma;
	int L = size - 1;
	path.resize(L + 1);
	double max = gamma(0, L);
	path[L] = 0;
	for (int k = 1; k < nstates; k++)
		if (max < gamma(k, L)) {
			max = gamma(k, L);
			path[L] = k;
		}
	for (int i = L; i >= 1; i--) {
		path[i - 1] = psi(path[i], i);
	}
	for (int i = 0; i < L; i++) {
		path[i] = path[i + 1];
	}
	for (int i = 0; i < L; i++) {
		path[i] = getState(path[i + 1])->getName()->id();
	}
	return max;

}

void HiddenMarkovModel::scale(std::vector<double> & in, int t) {
	double sum = 0.0;
	for (int i = 0; i < (int) in.size(); i++) {
		sum += in[i];
	}
	_ctFactors[t] = sum;
	for (int i = 0; i < (int) in.size(); i++) {
		in[i] = in[i] / sum;
	}
}

void HiddenMarkovModel::trainBaumWelch(SequenceList & sample, int maxiterations, double diff_threshold) {
	int nstates = _states.size();
	int alphabet_size = alphabet()->size();

	double diff = 10.0;
	if (maxiterations < 0)
		maxiterations = 500;

	for (int s = 0; s < (int) sample.size(); s++) {
		double last = 10000;
		for (int iterations = 0; iterations < maxiterations; iterations++) {

			Matrix A(nstates, nstates);
			Matrix E(nstates, alphabet_size);
			Matrix pi(nstates, 1);

			Matrix alpha(nstates, sample[s].size());
			Matrix beta(nstates, sample[s].size());

			double P = forward(sample[s], alpha);
			backward(sample[s], beta);

			double sum = alpha(0, 0) + beta(0, 0);
			for (int i = 1; i < nstates; i++)
				sum = log_sum(sum, alpha(i, 0) + beta(i, 0));

			for (int i = 0; i < nstates; i++) {
				pi(i, 0) = alpha(i, 0) + beta(i, 0) - sum;
			}

			for (int i = 0; i < nstates; i++) {
				for (int j = 0; j < nstates; j++) {
					int t = 0;
					double sum = -HUGE;
					if (t < (int) sample[s].size() - 1) {
						sum = alpha(i, t) + getState(i)->transitions()->log_probability_of(j) + getState(j)->emission()->log_probability_of(sample[s][t + 1]) + beta(j, t + 1);
						for (t = 1; t < (int) sample[s].size() - 1; t++) {
							sum = log_sum(sum,
									alpha(i, t) + getState(i)->transitions()->log_probability_of(j) + getState(j)->emission()->log_probability_of(sample[s][t + 1])
											+ beta(j, t + 1));
						}
					}
					A(i, j) = sum;
				}
				for (int sigma = 0; sigma < alphabet_size; sigma++) {
					int t = 0;
					double sum = -HUGE;
					bool first = true;
					for (t = 0; t < (int) sample[s].size(); t++) {
						if ((sigma == sample[s][t]) && first) {
							sum = alpha(i, t) + beta(i, t);
							first = false;
						} else if (sigma == sample[s][t]) {
							sum = log_sum(sum, alpha(i, t) + beta(i, t));
						}
					}
					E(i, sigma) = sum;
				}
			}

			Matrix sumA(1, nstates);
			Matrix sumE(1, alphabet_size);
			for (int k = 0; k < nstates; k++) {
				int l = 0;
				if (l < nstates) {
					sumA(0, k) = A(k, l);
					for (l = 1; l < nstates; l++)
						sumA(0, k) = log_sum(sumA(0, k), A(k, l));
				}
				int b = 0;
				if (b < alphabet_size) {
					sumE(0, k) = E(k, b);
					for (b = 1; b < alphabet_size; b++)
						sumE(0, k) = log_sum(sumE(0, k), E(k, b));
				}
			}


			std::vector<double> probs;
			probs.resize(nstates);
			for (int k = 0; k < nstates; k++) {
				_initial_probability->log_probability_of(k, pi(k, 0));
				for (int l = 0; l < nstates; l++) {
					A(k, l) = A(k, l) - sumA(0, k);
					getState(k)->transitions()->log_probability_of(l, A(k, l));
				}
				for (int b = 0; b < alphabet_size; b++) {
					E(k, b) = E(k, b) - sumE(0, k);
					getState(k)->emission()->log_probability_of(b, E(k, b));
				}
			}

			diff = fabs(last - P);
			if (diff < diff_threshold)
				break;
#if 0
			std::cerr << "iteration: " << iterations << std::endl;
			fprintf(stderr, "LL: %lf\n" , P );
			std::cerr << "Diff: " << diff << std::endl;
#endif
			last = P;
		}
	}

}

std::string HiddenMarkovModel::str() const {
	int nstates = _states.size();
	std::stringstream out;
	out << "model_name = \"" << model_name() << "\"" << std::endl;
	out << "state_names = (";
	out << "\"" << getStateName(0) << "\"";
	for (int i = 1; i < (int) getStateNames()->size(); i++)
		out << ",\"" << getStateName(i) << "\"";
	out << ")" << std::endl;

	out << "observation_symbols = (";
	out << "\"" << alphabet()->getSymbol(0)->name() << "\"";
	for (int i = 1; i < (int) alphabet()->size(); i++)
		out << ",\"" << alphabet()->getSymbol(i)->name() << "\"";
	out << ")" << std::endl;

	out << "transitions = (";
	out << "\"" << getStateName(0) << "\" | \"" << getStateName(0) << "\": " << exp(getState(0)->transitions()->log_probability_of(0));
	for (int i = 0; i < nstates; i++)
		for (int j = 0; j < nstates; j++)
			if ((i != 0) || (j != 0))
				out << ";\n \"" << getStateName(j) << "\" | \"" << getStateName(i) << "\": " << exp(getState(i)->transitions()->log_probability_of(j));
	out << ")" << std::endl;

	out << "emission_probabilities = (";
	out << "\"" << alphabet()->getSymbol(0)->name() << "\" | \"" << getStateName(0) << "\": " << exp(getState(0)->emission()->log_probability_of(0));
	for (int i = 0; i < nstates; i++)
		for (int j = 0; j < (int) alphabet()->size(); j++)
			if ((i != 0) || (j != 0))
				out << ";\n \"" << alphabet()->getSymbol(j)->name() << "\" | \"" << getStateName(i) << "\": " << exp(getState(i)->emission()->log_probability_of(j));
	out << ")" << std::endl;

	double sum = 0;
	std::vector<double> probs;
	probs.resize(nstates);
	for (int i = 0; i < nstates; i++) {
		probs[i] = exp(_initial_probability->log_probability_of(i));
		sum += probs[i];
	}

	out << "initial_probabilities = (";
	out << "\"" << getStateName(0) << "\":  " << probs[0] / sum;
	for (int i = 1; i < nstates; i++)
		out << ";\n \"" << getStateName(i) << "\": " << probs[i] / sum;
	out << ")" << std::endl;
	return out.str();
}

void HiddenMarkovModel::initialize(const ProbabilisticModelParameters & parameters) {
	ProbabilisticModelParameterValuePtr state_names = parameters.getMandatoryParameterValue("state_names");
	ProbabilisticModelParameterValuePtr observation_symbols = parameters.getMandatoryParameterValue("observation_symbols");
	ProbabilisticModelParameterValuePtr initial_probabilities = parameters.getMandatoryParameterValue("initial_probabilities");
	ProbabilisticModelParameterValuePtr transitions = parameters.getMandatoryParameterValue("transitions");
	ProbabilisticModelParameterValuePtr emissions = parameters.getMandatoryParameterValue("emission_probabilities");

	std::vector<HMMStatePtr> state_list;
	AlphabetPtr states = AlphabetPtr(new Alphabet());
	AlphabetPtr observations = AlphabetPtr(new Alphabet());
	states->initializeFromVector(state_names->getStringVector());
	observations->initializeFromVector(observation_symbols->getStringVector());

	std::map<std::string, double>::const_iterator it;
	std::map<std::string, std::vector<double> >::const_iterator it2;

	DiscreteIIDModelPtr pi = DiscreteIIDModelPtr(new DiscreteIIDModel());
	pi->initializeFromMap(initial_probabilities->getDoubleMap(), states);

	std::map<std::string, double> emisspar = emissions->getDoubleMap();
	std::map<std::string, double> transpar = transitions->getDoubleMap();

	std::map<std::string, DoubleVector> emiss;
	std::map<std::string, DoubleVector> trans;

	for (it = emisspar.begin(); it != emisspar.end(); it++) {
		std::vector<std::string> splited;
		boost::regex separator("\\|");
		split_regex(it->first, splited, separator);
		if (splited.size() == 1)
			splited.push_back("");
		std::string symbol(splited[0]);
		std::string state(splited[1]);

		std::map<std::string, DoubleVector>::iterator eit;
		eit = emiss.find(state);
		if (eit == emiss.end()) {
			int id = observations->getSymbol(symbol)->id();
			emiss[state].resize(observations->size());
			if ((id >= 0) && (id < (int) (emiss[state]).size()))
				(emiss[state])[id] = it->second;
		} else {
			int id = observations->getSymbol(symbol)->id();
			if ((id >= 0) && (id < (int) (eit->second).size()))
				(eit->second)[id] = it->second;
		}
	}

	for (it = transpar.begin(); it != transpar.end(); it++) {
		std::vector<std::string> splited;
		boost::regex separator("\\|");
		split_regex(it->first, splited, separator);
		if (splited.size() == 1)
			splited.push_back("");

		std::string to(splited[0]);
		std::string from(splited[1]);
		if (trans.find(from) == trans.end()) {
			int id = states->getSymbol(to)->id();
			DoubleVector probs;
			probs.resize(states->size());
			trans[from] = probs;
			if (id < (int) trans[from].size())
				trans[from][id] = it->second;
		} else {
			int id = states->getSymbol(to)->id();
			if (id < (int) trans[from].size())
				trans[from][id] = it->second;
		}
	}
	for (unsigned int i = 0; i < states->size(); i++) {
		SymbolPtr state_name = states->getSymbol(i);
		DiscreteIIDModelPtr e;
		DiscreteIIDModelPtr t;
		it2 = emiss.find(state_name->name());
		if (it2 != emiss.end())
			e = DiscreteIIDModelPtr(new DiscreteIIDModel(it2->second));
		it2 = trans.find(state_name->name());
		if (it2 != trans.end())
			t = DiscreteIIDModelPtr(new DiscreteIIDModel(it2->second));
		else {
			std::cerr << "ERROR: Could not configure the state " << state_name->name() << "!" << std::endl;
			exit(-1);
		}
		HMMStatePtr statePtr = HMMStatePtr(new HMMState(state_list.size(), state_name, e, t));
		state_list.push_back(statePtr);
	}

	setStates(state_list, states);
	setInitialProbability(pi);
	setObservationSymbols(observations);
}

ProbabilisticModelParameters HiddenMarkovModel::parameters() const {
	ProbabilisticModelParameters answer;
	int nstates = _states.size();
	answer.add("model_name", StringParameterValuePtr(new StringParameterValue(model_name().c_str())));
	answer.add("state_names", _state_names->getParameterValue());
	answer.add("observation_symbols", alphabet()->getParameterValue());
	std::map<std::string, double> trans;
	std::stringstream out;
	out << getStateName(0) << "|" << getStateName(0);
	trans[out.str()] = exp(getState(0)->transitions()->log_probability_of(0));
	for (int i = 0; i < nstates; i++)
		for (int j = 0; j < nstates; j++)
			if ((i != 0) || (j != 0)) {
				std::stringstream out2;
				out2 << getStateName(j) << "|" << getStateName(i);
				trans[out2.str()] = exp(getState(i)->transitions()->log_probability_of(j));
			}
	answer.add("transitions", DoubleMapParameterValuePtr(new DoubleMapParameterValue(trans)));

	std::map<std::string, double> emission;
	std::stringstream out3;
	out3 << alphabet()->getSymbol(0)->name() << "|" << getStateName(0);
	emission[out3.str()] = exp(getState(0)->emission()->log_probability_of(0));
	for (int i = 0; i < nstates; i++)
		for (int j = 0; j < (int) alphabet()->size(); j++)
			if ((i != 0) || (j != 0)) {
				std::stringstream out4;
				out4 << alphabet()->getSymbol(j)->name() << "|" << getStateName(i);
				emission[out4.str()] = exp(getState(i)->emission()->log_probability_of(j));
			}
	answer.add("emission_probabilities", DoubleMapParameterValuePtr(new DoubleMapParameterValue(emission)));
	double sum = 0;
	std::vector<double> probs;
	probs.resize(nstates);
	for (int i = 0; i < nstates; i++) {
		probs[i] = exp(_initial_probability->log_probability_of(i));
		sum += probs[i];
	}
	std::map<std::string, double> initial;
	std::stringstream out5;
	out5 << getStateName(0);
	initial[out5.str()] = probs[0] / sum;
	for (int i = 0; i < nstates; i++)
		for (int j = 0; j < (int) alphabet()->size(); j++)
			if ((i != 0) || (j != 0)) {
				std::stringstream out6;
				out6 << getStateName(i);
				initial[out6.str()] = probs[i] / sum;
			}
	answer.add("initial_probabilities", DoubleMapParameterValuePtr(new DoubleMapParameterValue(initial)));
	return answer;

}

void HiddenMarkovModel::setInitialProbability(DiscreteIIDModelPtr initial) {
	_initial_probability = initial;
}
void HiddenMarkovModel::setObservationSymbols(AlphabetPtr obs) {
	tops::ProbabilisticModel::setAlphabet(obs);
}
void HiddenMarkovModel::setStates(std::vector<HMMStatePtr> states, AlphabetPtr state_names) {
	_states = states;
	_state_names = state_names;
}

}
