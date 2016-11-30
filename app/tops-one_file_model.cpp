/*
 *       one_file_model.cpp
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

#include <boost/program_options.hpp>

#include <map>
#include <iostream>
#include <vector>
#include <string>

#include "GeneralizedHiddenMarkovModelCreator.hpp"
#include "ProbabilisticModelCreatorClient.hpp"

#include <fstream>
#include <iostream>
#include "version.hpp"
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
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << desc << "\n";
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
                    cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
                    cout << std::endl;
                    cout << desc << endl;
                    exit(-1);
                }
        } catch (boost::program_options::invalid_command_line_syntax &e) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            std::cout << "error: " << e.what() << std::endl;
            cout << desc << endl;
        } catch (boost::program_options::unknown_option &e) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            std::cout << "error: " << e.what() << std::endl;
            cout << desc << endl;
        } catch (boost::bad_any_cast & e) {
            cout << argv[0] << ": ToPS version " << APP_VERSION << std::endl;
            cout << std::endl;
            cout << desc << endl;
        }

        return 0;
}
