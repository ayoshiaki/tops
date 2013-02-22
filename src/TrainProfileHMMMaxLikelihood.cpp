/*
 *       TrainProfileHMMMaxLikelihood.cpp
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

#include "ProbabilisticModel.hpp"
#include "ProbabilisticModelCreator.hpp"
#include "ConfigurationReader.hpp"
#include "TrainProfileHMMMaxLikelihood.hpp"
#include "ProfileHiddenMarkovModel.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
namespace tops {

ProbabilisticModelPtr TrainProfileHMMMaxLikelihood::create(ProbabilisticModelParameters & parameters) const {

	string datetime = to_iso_string(boost::posix_time::microsec_clock::local_time()).substr(0, 15);

	ProbabilisticModelParameterValuePtr fastafilepathpar = parameters.getMandatoryParameterValue("fasta_file");
	ProbabilisticModelParameterValuePtr pseudocountpar = parameters.getMandatoryParameterValue("pseudocounts");
	ProbabilisticModelParameterValuePtr residuefractionpar = parameters.getMandatoryParameterValue("residue_fraction");
	ProbabilisticModelParameterValuePtr alphabetpar = parameters.getMandatoryParameterValue("alphabet");

	vector<string> symbols;
	string alphabet = alphabetpar->getString();
	boost::split(symbols, alphabet, boost::is_any_of(","));

	int pseudocounts = 1;
	if (pseudocountpar != NULL)
		pseudocounts = pseudocountpar->getInt();

	infer_model(fastafilepathpar->getString(), pseudocountpar->getInt(), residuefractionpar->getDouble(), datetime, symbols);

//    ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
//    ProbabilisticModelParameterValuePtr statespar = parameters.getMandatoryParameterValue("states_training_set");
//    ProbabilisticModelParameterValuePtr emisspar = parameters.getMandatoryParameterValue("emissions_training_set");
//    ProbabilisticModelParameterValuePtr pseudopar = parameters.getOptionalParameterValue("pseudocounts");

	string emiss_filename = ".emiss_" + datetime + ".txt";
	string states_filename = ".states_" + datetime + ".txt";
	string model_filename = ".model_" + datetime + ".txt";
	string config_filename = ".config_" + datetime + ".txt";

	ProbabilisticModelCreatorClient creator;
//    std::string name = initmodelpar->getString();
	ProbabilisticModelPtr m = creator.create(model_filename);
	SequenceEntryList states_sample_set, emiss_sample_set;
	AlphabetPtr emiss_alphabet = m->alphabet();
	AlphabetPtr states_alphabet = m->getStateNames();
	readSequencesFromFile(states_sample_set, states_alphabet, states_filename);
	readSequencesFromFile(emiss_sample_set, emiss_alphabet, emiss_filename);

	SequenceList seqs1, seqs2;
	for (int i = 0; i < (int) states_sample_set.size(); i++)
		seqs1.push_back(states_sample_set[i]->getSequence());
	for (int i = 0; i < (int) emiss_sample_set.size(); i++)
		seqs2.push_back(emiss_sample_set[i]->getSequence());
	m->profileDecodable()->trainMaxLikelihood(seqs1, seqs2, pseudocounts);

	string remover = "rm " + config_filename + " " + model_filename + " " + states_filename + " " + emiss_filename;

	system(remover.c_str());

	return m;
}

void TrainProfileHMMMaxLikelihood::infer_model(string fastafile, int pseudocounts, double residuefraction, string datetime, const vector<string>& symbols) const {
	string line, sequencia;
	vector<string> seq;
	vector<string> seq_names;

	ifstream myfile(fastafile.c_str());

	if (myfile.is_open()) {
		while (!myfile.eof()) {
			getline(myfile, line);
			if (match_expression(line, ">.*")) {
				if (seq_names.size() != 0) {
					seq.push_back(sequencia);
					sequencia = "";
				}
				seq_names.push_back(line.substr(1, line.length()));
			} else {
				sequencia += line;
			}
		}
		if (seq_names.size() != 0) {
			seq.push_back(sequencia);
			sequencia = "";
		}
		myfile.close();

	}

	int count[seq[0].size()];
	bool match[seq[0].size()];
	int index[seq[0].size()];
	int pos = 0;
	int n_match_states = 0;

	for (unsigned int z = 0; z < seq[0].size(); z++) {
		count[z] = 0;
	}
	pos = 0;
	for (unsigned int j = 0; j < seq[0].size(); j++) {
		unsigned int i;
		for (i = 0; i < seq.size(); i++) {
			if (seq[i][j] != '.' && seq[i][j] != '-') {
				count[j]++;
			}
		}
		if (count[j] > seq.size() * residuefraction) {
			match[j] = true;
			index[j] = ++pos;
			n_match_states++;
		} else {
			match[j] = false;
			index[j] = pos;
		}
	}
	string emiss_filename = ".emiss_" + datetime + ".txt";
	string states_filename = ".states_" + datetime + ".txt";
	string model_filename = ".model_" + datetime + ".txt";
	string config_filename = ".config_" + datetime + ".txt";

	config_file_creator(states_filename, emiss_filename, model_filename, config_filename, pseudocounts);
	model_file_creator(n_match_states, symbols, model_filename);
	emiss_states_file_creator(seq, index, match, seq_names, emiss_filename, states_filename);

}

bool TrainProfileHMMMaxLikelihood::match_expression(string input, string er) const {
	boost::regex e(er);
	return boost::regex_match(input, e);
}

void TrainProfileHMMMaxLikelihood::model_file_creator(int n_match_states, const vector<string>& symbols, string model_filename) const {

	ofstream model;

	model.open(model_filename.c_str());
	model << "model_name = \"ProfileHiddenMarkovModel\"" << endl;
	model << "state_names = (\"M0\"";

	for (int i = 1; i < n_match_states + 2; i++) {
		model << ",\"M" << i << "\"";
	}
	for (int i = 0; i < n_match_states + 1; i++) {
		model << ",\"I" << i << "\"";
	}
	for (int i = 1; i < n_match_states + 1; i++) {
		model << ",\"D" << i << "\"";
	}
	model << ")" << endl;

	model << "observation_symbols = (";
	string str = "";
	for (unsigned int i = 0; i < symbols.size(); i++) {
		str += "\"" + symbols[i] + "\",";
	}
	model << str.substr(0, str.length() - 1) << ")" << endl;

	model << "transitions = (\"I0\" | \"M0\": 0.33333;" << endl;
	model << "\t\t\"D1\" | \"M0\": 0.33333;" << endl;
	model << "\t\t\"M1\" | \"M0\": 0.33333;" << endl;
	model << "\t\t\"D1\" | \"I0\": 0.33333;" << endl;
	model << "\t\t\"I0\" | \"I0\": 0.33333;" << endl;
	model << "\t\t\"M1\" | \"I0\": 0.33333;" << endl;

	for (int i = 1; i < n_match_states; i++) {
		int next = i + 1;
		model << "\t\t\"I" << i << "\" | " << "\"M" << i << "\": 0.33333;" << endl;
		model << "\t\t\"D" << next << "\" | " << "\"M" << i << "\": 0.33333;" << endl;
		model << "\t\t\"M" << next << "\" | " << "\"M" << i << "\": 0.33333;" << endl;
		model << "\t\t\"D" << next << "\" | " << "\"I" << i << "\": 0.33333;" << endl;
		model << "\t\t\"I" << i << "\" | " << "\"I" << i << "\": 0.33333;" << endl;
		model << "\t\t\"M" << next << "\" | " << "\"I" << i << "\": 0.33333;" << endl;
		model << "\t\t\"D" << next << "\" | " << "\"D" << i << "\": 0.33333;" << endl;
		model << "\t\t\"I" << i << "\" | " << "\"D" << i << "\": 0.33333;" << endl;
		model << "\t\t\"M" << next << "\" | " << "\"D" << i << "\": 0.33333;" << endl;
	}

	int last = n_match_states + 1;
	int prev = n_match_states;

	model << "\t\t\"I" << prev << "\" | " << "\"M" << prev << "\": 0.5;" << endl;
	model << "\t\t\"M" << last << "\" | " << "\"M" << prev << "\": 0.5;" << endl;
	model << "\t\t\"I" << prev << "\" | " << "\"I" << prev << "\": 0.5;" << endl;
	model << "\t\t\"M" << last << "\" | " << "\"I" << prev << "\": 0.5;" << endl;
	model << "\t\t\"I" << prev << "\" | " << "\"D" << prev << "\": 0.5;" << endl;
	model << "\t\t\"M" << last << "\" | " << "\"D" << prev << "\": 0.5;" << endl;
	model << "\t\t\"M" << last << "\" | " << "\"M" << last << "\": 1.0)" << endl;

	long double prob = 1.0 / symbols.size();
	model << "emission_probabilities = (";
	string state = "I0";
	for (unsigned int j = 0; j < symbols.size(); j++) {
		char sbl = symbols[j][0];
		model << "\"" << sbl << "\" | \"" << state << "\": " << setprecision(10) << prob << ";" << endl;
	}
	for (int i = 1; i < n_match_states + 1; i++) {
		for (unsigned int j = 0; j < symbols.size(); j++) {
			char sbl = symbols[j][0];
			model << "\"" << sbl << "\" | \"" << "M" << i << "\": " << setprecision(10) << prob << ";" << endl;
		}
		for (unsigned int j = 0; j < symbols.size(); j++) {
			char sbl = symbols[j][0];
			if (j < symbols.size() - 1 || i < n_match_states) {
				model << "\"" << sbl << "\" | \"" << "I" << i << "\": " << setprecision(10) << prob << ";" << endl;
			} else {
				model << "\"" << sbl << "\" | \"" << "I" << i << "\": " << setprecision(10) << prob << ")" << endl;
			}
		}
	}
	model << "initial_probabilities= (\"M0\": 1.0)" << endl;
	model.close();
}

void TrainProfileHMMMaxLikelihood::config_file_creator(string states_filename, string emiss_filename, string model_filename, string config_filename, int pseudocounts) const {

	ofstream config;

	config.open(config_filename.c_str());
	config << "training_algorithm=\"ProfileHMMMaxLikelihood\"" << endl;
	config << "states_training_set=\"" << states_filename << "\"" << endl;
	config << "emissions_training_set=\"" << emiss_filename << "\"" << endl;
	config << "initial_model=\"" << model_filename << "\"" << endl;
	config << "pseudocounts=" << pseudocounts << endl;

	config.close();
}

void TrainProfileHMMMaxLikelihood::emiss_states_file_creator(const vector<string>& seq, int index[], bool match[], const vector<string>& seq_names, string emiss_filename,
		string states_filename) const {
	ofstream emiss, states;
	emiss.open(emiss_filename.c_str());
	states.open(states_filename.c_str());
	for (unsigned int i = 0; i < seq.size(); i++) {
		emiss << seq_names[i] << "_emiss: ";
		states << seq_names[i] << "_states: ";
		states << "M0 ";
		for (unsigned int j = 0; j < seq[i].size(); j++) {
			if (match[j]) {
				if (seq[i][j] == '.' || seq[i][j] == '-') {
					states << "D" << index[j] << " ";
				} else {
					emiss << seq[i][j] << " ";
					states << "M" << index[j] << " ";
				}
			} else {
				if (!(seq[i][j] == '.' || seq[i][j] == '-')) {
					emiss << seq[i][j] << " ";
					states << "I" << index[j] << " ";
				}
			}
		}
		states << "M" << index[seq[0].size() - 1] + 1;
		emiss << endl;
		states << endl;
	}
	emiss.close();
	states.close();
}

}
;

