/*
 *       DiscreteIIDModelCreateModel.cpp
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

#include "Alphabet.h"
#include "DiscreteIIDModelFactory.h"
#include "DiscreteIIDModelCreateModel.h"
#include "ReadConfigurationFile.h"
#include "SequenceFactory.h"
#include "FASTAReader.h"
#include <string>


namespace myop {

  ProbabilisticModelPtr DiscreteIIDModelCreateModel::create (const std::string & config) {
    std::string TYPE("distribution");
    std::vector <std::string> mandatory;
    mandatory.push_back(TYPE);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);

    std::map <std::string, DiscreteIIDModelCreateModelPtr> commands;
    std::string BERNOULLI("Bernoulli");
    std::string UNIFORM_ALPHABET("Uniform");
    std::string SMOOTHED_HISTOGRAM_BURGE("SmoothedHistogramBurge");
    std::string SMOOTHED_HISTOGRAM_STANKE("SmoothedHistogramStanke");
    std::string SMOOTHED_HISTOGRAM_SHEATHER_JONES("SmoothedHistogramSheaterJones");
    std::string SMOOTHED_HISTOGRAM_MYOP("SmoothedHistogramMYOP");
    std::string SMOOTHED_HISTOGRAM_MAJOROS("SmoothedHistogramMajoros");

    commands[BERNOULLI] = DiscreteIIDModelCreateModelPtr(new BernoulliCreateModel());
    commands[UNIFORM_ALPHABET] = DiscreteIIDModelCreateModelPtr(new UniformAlphabetCreateModel());
    commands[SMOOTHED_HISTOGRAM_BURGE] = DiscreteIIDModelCreateModelPtr(new SmoothedHistogramBurgeCreateModel());
    commands[SMOOTHED_HISTOGRAM_STANKE] = DiscreteIIDModelCreateModelPtr(new SmoothedHistogramStankeCreateModel());
    commands[SMOOTHED_HISTOGRAM_SHEATHER_JONES] = DiscreteIIDModelCreateModelPtr(new SmoothedHistogramKernelDensityCreateModel());
    commands[SMOOTHED_HISTOGRAM_MYOP] = DiscreteIIDModelCreateModelPtr(new SmoothedHistogramMYOPCreateModel());
    commands[SMOOTHED_HISTOGRAM_MAJOROS] = DiscreteIIDModelCreateModelPtr(new SmoothedHistogramMajorosCreateModel());

    if(commands.find(conf[TYPE]) == commands.end())
      {
        std::cerr << "Invalid value for \"distribution\" parameter" << std::endl;
        std::cerr << "Valid values are: " << std::endl;
        std::map<std::string,DiscreteIIDModelCreateModelPtr> :: iterator it;
        for (it = commands.begin(); it != commands.end(); it++)
          std::cerr << "\t" << it->first << std::endl;
        exit(-1);
      }
    return commands[conf[TYPE]]->create(config);
  }

  ProbabilisticModelPtr BernoulliCreateModel::create (const std::string & config) {
    std::string PROBABILITY ("probability");
    std::vector <std::string> mandatory;
    mandatory.push_back(PROBABILITY);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DiscreteIIDModelFactory factory;
    return factory.bernoulli(atof(conf[PROBABILITY].c_str()));
  }

  ProbabilisticModelPtr UniformAlphabetCreateModel::create (const std::string & config) {
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    DiscreteIIDModelFactory factory;
    return factory.uniform(alphabet);
  }

  ProbabilisticModelPtr SmoothedHistogramBurgeCreateModel::create (const std::string & config) {
    std::string TRAINING_SET("trainig_set");
    std::string C("C");
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(TRAINING_SET);
    mandatory.push_back(C);
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DoubleVector lengths;

    FASTAReader reader;
    if(!reader.open(conf[TRAINING_SET]))
      {
        std::cerr << "Can't open file: " << conf[TRAINING_SET] << std::endl;
        exit(-1);
      }
    std::string sequence;
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    SequenceFactory seqFactory(alphabet);
    while(reader.nextSequence(sequence))
      {
        Sequence sample;
        seqFactory.createSequence(sample, sequence);
        lengths.push_back((double)sequence.size());
      }
    reader.close();
    DiscreteIIDModelFactory factory;
    return factory.smoothedDistributionBurge(lengths, atof(conf[C].c_str()));
  }

  ProbabilisticModelPtr SmoothedHistogramMajorosCreateModel::create (const std::string & config) {
    std::string TRAINING_SET("trainig_set");
    std::string WINDOW_SIZE("window_size");
    std::string INTERACTIONS("interactions");
    std::string N("N");
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(TRAINING_SET);
    mandatory.push_back(WINDOW_SIZE);
    mandatory.push_back(INTERACTIONS);
    mandatory.push_back(N);
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DoubleVector lengths;

    FASTAReader reader;

    if(!reader.open(conf[TRAINING_SET]))
      {
        std::cerr << "Can't open file: " << conf[TRAINING_SET] << std::endl;
        exit(-1);
      }
    std::string sequence;
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    SequenceFactory seqFactory(alphabet);
    while(reader.nextSequence(sequence))
      {
        Sequence sample;
        seqFactory.createSequence(sample, sequence);
        lengths.push_back((double)sequence.size());
      }
    reader.close();
    DiscreteIIDModelFactory factory;
    return factory.smoothedDistributionMajoros(lengths, atoi(conf[WINDOW_SIZE].c_str()), atoi(conf[INTERACTIONS].c_str()), atoi(conf[N].c_str()));
  }


  ProbabilisticModelPtr SmoothedHistogramStankeCreateModel::create (const std::string & config)
  {
    std::string TRAINING_SET("trainig_set");
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(TRAINING_SET);
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DoubleVector lengths;

    FASTAReader reader ;

    if(!reader.open(conf[TRAINING_SET]))
      {
        std::cerr << "Can't open file: " << conf[TRAINING_SET] << std::endl;
        exit(-1);
      }
    std::string sequence;
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    SequenceFactory seqFactory(alphabet);
    while(reader.nextSequence(sequence))
      {
        Sequence sample;
        seqFactory.createSequence(sample, sequence);
        lengths.push_back(sequence.size());
      }
    reader.close();
    DiscreteIIDModelFactory factory;
    return factory.smoothedDistributionKernelDensityStanke(lengths);
  }


  ProbabilisticModelPtr SmoothedHistogramKernelDensityCreateModel::create (const std::string & config) {
    std::string TRAINING_SET("trainig_set");
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(TRAINING_SET);
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DoubleVector lengths;
    std::ifstream is;
    FASTAReader reader;

    if(!reader.open(conf[TRAINING_SET]))
      {
        std::cerr << "Can't open file: " << conf[TRAINING_SET] << std::endl;
        exit(-1);
      }
    std::string sequence;
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    SequenceFactory seqFactory(alphabet);
    while(reader.nextSequence(sequence))
      {
        Sequence sample;
        seqFactory.createSequence(sample, sequence);
        lengths.push_back(sequence.size());
      }
    reader.close();
    DiscreteIIDModelFactory factory;
    return factory.smoothedDistributionKernelDensity(lengths);
  }


  ProbabilisticModelPtr SmoothedHistogramMYOPCreateModel::create (const std::string & config) {
    std::string TRAINING_SET("trainig_set");
    std::string ALPHABET("alphabet");
    std::vector <std::string> mandatory;
    mandatory.push_back(TRAINING_SET);
    mandatory.push_back(ALPHABET);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);
    DoubleVector lengths;
    FASTAReader reader;

    if(!reader.open(conf[TRAINING_SET]))
      {
        std::cerr << "Can't open file: " << conf[TRAINING_SET] << std::endl;
        exit(-1);
      }
    std::string sequence;
    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromString(conf[ALPHABET]);
    SequenceFactory seqFactory(alphabet);
    while(reader.nextSequence(sequence))
      {
        Sequence sample;
        seqFactory.createSequence(sample, sequence);
        lengths.push_back(sequence.size());
      }
    reader.close();
    DiscreteIIDModelFactory factory;
    return factory.smoothedDistributionKernelDensityMYOP(lengths);

  }
}

