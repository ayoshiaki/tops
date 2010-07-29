#include <boost/program_options.hpp>

#include <map>
#include <iostream>
#include <vector>
#include <string>

#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "ProbabilisticModelCreatorClient.hpp"

#include <fstream>
#include <iostream>

using namespace tops;
using namespace std;
using namespace boost::program_options;

int main(int argc, char ** argv) {

	options_description desc("Allowed options");
	desc.add_options()("help,h", "produce help message")("configuration,c",
			value<string> (), "configuration file")("output,o",
			value<string> (), "output file");

	try {
		variables_map vm;
		store(parse_command_line(argc, argv, desc), vm);
		notify(vm);

		if (vm.count("help")) {
			cerr << desc << "\n";
			return 1;
		}

		if (vm.count("configuration")) {
			string file = vm["configuration"].as<string> ();
			ConfigurationReader readConfig;

			std::ifstream input;
			std::string line;

			input.open(file.c_str());
			if (!input.is_open()) {
				std::cerr << "Cant open file " << file << std::endl;
				exit(-1);
			}
			string conf;
			while (!input.eof()) {
				getline(input, line, '\n');
				line += "\n";
				conf.append(line);
			}
			input.close();
			if (readConfig.load(conf)) {
			  ProbabilisticModelCreatorClient creator;
			  std::cout << creator.create(*(readConfig.parameters()))->str() << std::endl;
			}
		} else {
			cerr << desc << endl;
			exit(-1);
		}
	} catch (boost::program_options::invalid_command_line_syntax &e) {
		std::cerr << "error: " << e.what() << std::endl;
		cerr << desc << endl;
	} catch (boost::program_options::unknown_option &e) {
		std::cerr << "error: " << e.what() << std::endl;
		cerr << desc << endl;
	} catch (boost::bad_any_cast & e) {
		cerr << desc << endl;
	}

	return 0;
}
