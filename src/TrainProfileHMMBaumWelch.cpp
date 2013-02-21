/*
 *       TrainProfileHMMBaumWelch.cpp
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
#include "TrainProfileHMMBaumWelch.hpp"
#include "ProfileHiddenMarkovModel.hpp"
#include "util.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
namespace tops {

ProbabilisticModelPtr TrainProfileHMMBaumWelch::create(ProbabilisticModelParameters & parameters) const {

	string datetime = to_iso_string(boost::posix_time::microsec_clock::local_time()).substr(0, 15);

	ProbabilisticModelParameterValuePtr initmodelpar = parameters.getMandatoryParameterValue("initial_model");
	ProbabilisticModelParameterValuePtr fastafilepar = parameters.getMandatoryParameterValue("training_set");
	ProbabilisticModelParameterValuePtr thresholdpar = parameters.getMandatoryParameterValue("threshold");
	ProbabilisticModelParameterValuePtr maxiterpar = parameters.getOptionalParameterValue("maxiter");
	ProbabilisticModelParameterValuePtr pseudocountpar = parameters.getOptionalParameterValue("pseudocounts");

	create_emiss_file(fastafilepar->getString(), datetime);

	int pseudocounts = 1;
	if (pseudocountpar != NULL)
		pseudocounts = pseudocountpar->getInt();

	ProbabilisticModelCreatorClient creator;
	std::string name = initmodelpar->getString();
	ProbabilisticModelPtr m = creator.create(name);
	SequenceEntryList states_sample_set, emiss_sample_set;
	AlphabetPtr emiss_alphabet = m->alphabet();
	AlphabetPtr states_alphabet = m->getStateNames();

	string emiss_filename = ".emiss_" + datetime + ".txt";

	readSequencesFromFile(emiss_sample_set, emiss_alphabet, emiss_filename);

	SequenceList seqs1;

	for (int i = 0; i < (int) emiss_sample_set.size(); i++)
		seqs1.push_back(emiss_sample_set[i]->getSequence());
	m->profileDecodable()->trainBaumWelch(seqs1, maxiterpar->getInt(), thresholdpar->getDouble(), pseudocounts);

	string remover = "rm " + emiss_filename;

	system(remover.c_str());
	return m;
}

bool TrainProfileHMMBaumWelch::match_expression(string input, string er) const {
	boost::regex e(er);
	return boost::regex_match(input, e);
}

void TrainProfileHMMBaumWelch::create_emiss_file(string fastafile, string datetime) const {
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

	string emiss_filename = ".emiss_" + datetime + ".txt";

	ofstream emiss;
	emiss.open(emiss_filename.c_str());
	for (unsigned int i = 0; i < seq.size(); i++) {
		emiss << seq_names[i] << "_emiss: ";
		for (unsigned int j = 0; j < seq[i].size(); j++) {

			if (seq[i][j] != '-')
				emiss << seq[i][j] << " ";

		}
		emiss << endl;

	}
	emiss.close();

}

}
;

