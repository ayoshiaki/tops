#include "Alphabet.h"
#include "MultinomialDistributionFactory.h"
#include "MultinomialDistributionCreateModel.h"
#include "ReadConfigurationFile.h"
#include "SequenceFactory.h"
#include "FASTAReader.h"
#include <string>


namespace myop {

  ProbabilisticModelPtr MultinomialDistributionCreateModel::create (const std::string & config) {
    std::string TYPE("distribution");
    std::vector <std::string> mandatory;
    mandatory.push_back(TYPE);
    ReadConfigurationFile readConf;
    Configuration conf = readConf.load_configuration_file(config, mandatory);

    std::map <std::string, MultinomialDistributionCreateModelPtr> commands;
    std::string BERNOULLI("Bernoulli");
    std::string UNIFORM_ALPHABET("Uniform");
    std::string SMOOTHED_HISTOGRAM_BURGE("SmoothedHistogramBurge");
    std::string SMOOTHED_HISTOGRAM_STANKE("SmoothedHistogramStanke");
    std::string SMOOTHED_HISTOGRAM_SHEATHER_JONES("SmoothedHistogramSheaterJones");
    std::string SMOOTHED_HISTOGRAM_MYOP("SmoothedHistogramMYOP");
    std::string SMOOTHED_HISTOGRAM_MAJOROS("SmoothedHistogramMajoros");
    
    commands[BERNOULLI] = MultinomialDistributionCreateModelPtr(new BernoulliCreateModel());
    commands[UNIFORM_ALPHABET] = MultinomialDistributionCreateModelPtr(new UniformAlphabetCreateModel());
    commands[SMOOTHED_HISTOGRAM_BURGE] = MultinomialDistributionCreateModelPtr(new SmoothedHistogramBurgeCreateModel());
    commands[SMOOTHED_HISTOGRAM_STANKE] = MultinomialDistributionCreateModelPtr(new SmoothedHistogramStankeCreateModel());
    commands[SMOOTHED_HISTOGRAM_SHEATHER_JONES] = MultinomialDistributionCreateModelPtr(new SmoothedHistogramKernelDensityCreateModel());
    commands[SMOOTHED_HISTOGRAM_MYOP] = MultinomialDistributionCreateModelPtr(new SmoothedHistogramMYOPCreateModel());
    commands[SMOOTHED_HISTOGRAM_MAJOROS] = MultinomialDistributionCreateModelPtr(new SmoothedHistogramMajorosCreateModel());
    
    if(commands.find(conf[TYPE]) == commands.end())
      {
	std::cerr << "Invalid value for \"distribution\" parameter" << std::endl;
	std::cerr << "Valid values are: " << std::endl;
	std::map<std::string,MultinomialDistributionCreateModelPtr> :: iterator it;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
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
    MultinomialDistributionFactory factory;
    return factory.smoothedDistributionKernelDensityMYOP(lengths);

  }
}

