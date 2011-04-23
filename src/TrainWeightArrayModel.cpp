#include "ContextTree.hpp"
#include "TrainWeightArrayModel.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "ProbabilisticModelParameter.hpp"
#include "ProbabilisticModelCreatorClient.hpp"
#include "SequenceFactory.hpp"
namespace tops{

  ProbabilisticModelPtr TrainWeightArrayModel::create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const
  {
    ProbabilisticModelParameterValuePtr alphabetpar = parameters.getMandatoryParameterValue("alphabet");
    ProbabilisticModelParameterValuePtr trainingsetpar = parameters.getMandatoryParameterValue("training_set");
    ProbabilisticModelParameterValuePtr orderpar = parameters.getMandatoryParameterValue("order");
    ProbabilisticModelParameterValuePtr lengthpar = parameters.getMandatoryParameterValue("length");
    ProbabilisticModelParameterValuePtr offsetpar = parameters.getOptionalParameterValue("offset");
    ProbabilisticModelParameterValuePtr vicinitypar = parameters.getOptionalParameterValue("vicinity_length");
    ProbabilisticModelParameterValuePtr pseudocountspar = parameters.getOptionalParameterValue("pseudo_counts");
    ProbabilisticModelParameterValuePtr fixseqpar = parameters.getOptionalParameterValue("fixed_sequence");
    ProbabilisticModelParameterValuePtr fixseqpospar = parameters.getOptionalParameterValue("fixed_sequence_position");
    ProbabilisticModelParameterValuePtr aprioripar = parameters.getOptionalParameterValue("apriori");

	ProbabilisticModelParameterValuePtr weightspar = parameters.getOptionalParameterValue("weights");
	std::map <std::string, double> weights;
	if(weightspar != NULL) {
	  readMapFromFile(weights, weightspar->getString());
	}


    if((alphabetpar == NULL) ||
       (trainingsetpar == NULL) ||
       (lengthpar == NULL) ||
       (orderpar == NULL) )
      {
        std::cerr << help() << std::endl;
        exit(-1);
      }
    int offset = 0;
    double pseudocounts = 0;
    if(pseudocountspar != NULL)
        pseudocounts = pseudocountspar->getDouble();
    ProbabilisticModelPtr apriori;
    if(aprioripar != NULL)
      {
         ProbabilisticModelCreatorClient c;
         apriori = c.create(aprioripar->getString());
      }

    if(offsetpar != NULL)
      offset = offsetpar ->getInt();
    if(pseudocountspar != NULL)
      pseudocounts = pseudocountspar->getDouble();

    AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
    alphabet->initializeFromVector(alphabetpar ->getStringVector());
    SequenceEntryList sample_set;
    readSequencesFromFile(sample_set, alphabet, trainingsetpar->getString());
    int vicinity = 0;
    if( vicinitypar != NULL)
      vicinity = vicinitypar->getInt();
    int order = orderpar->getInt();
    int length = lengthpar->getInt();
    std::vector <ContextTreePtr> positional_distribution;
    positional_distribution.resize(length);
    sample_size = 0;
    bool fixseq = false;
    Sequence fixed;
    int fixed_pos = 0;
    if((fixseqpar != NULL) && (fixseqpospar != NULL)) {
      std::string seqstr = fixseqpar->getString();
      SequenceFactory factory(alphabet);
      fixed = factory.createSequence(seqstr);
      fixed_pos = fixseqpospar->getInt();
      fixseq = true;
    }

    for(int i = 0; i < length; i++)
      {
        SequenceEntryList positionalSample;
        int o = i;
        if(o > order) o = order;
        for(int j = 0; j < (int)sample_set.size(); j++)
          {
              for(int k = -vicinity; k <= vicinity; k++)
                  {
                      Sequence s;
                      s.resize(o+1);
                      int l = 0;
                      int m = i - o + k+ offset;

                      if(m < 0)
                          continue;
                      if((m + o) >= (int)(sample_set[j]->getSequence()).size())
                          continue;


                      if(fixseq) {
                          while( (m < (int) (sample_set[j]->getSequence()).size()) && (l <= o))
                              {
                                  s[l] = (sample_set[j]->getSequence())[m];
                                  if(fixseq && (fixed_pos <= (m-k)) && ( (m-k) <= fixed_pos + fixed.size()-1)){
                                      int p = m-fixed_pos-k;
                                      if((p >= 0) && (p < fixed.size()))
                                          s[l] = fixed[p];
                                  }
                                  l++; m++;
                              }
                      } else {
                          while( (m < (int) (sample_set[j]->getSequence()).size()) && (l <= o))
                              {
                                  s[l] = (sample_set[j]->getSequence())[m];
                                  l++; m++;
                              }
                      }

                      SequenceEntryPtr entry = SequenceEntryPtr(new SequenceEntry(alphabet));
                      entry->setSequence(s);
                      entry->setName(sample_set[j]->getName());
                      positionalSample.push_back(entry);
                  }
          }
        if(fixseq && (fixed_pos <= i) && ((int)i <= ((int)fixed_pos + (int)fixed.size() - 1))){
            ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
            tree->initializeCounter(positionalSample, o, 0, weights);
            tree->normalize();
            positional_distribution[i] = tree;
        } else {
            ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));

            if(apriori != NULL  && apriori->factorable() != NULL){
	      tree->initializeCounter(positionalSample, o, pseudocounts, weights);
                tree->normalize(apriori, pseudocounts);
            } else {
	      tree->initializeCounter(positionalSample, o, pseudocounts, weights);
                tree->normalize();

            }
            positional_distribution[i] = tree;
        }

      }
    InhomogeneousMarkovChainPtr model = InhomogeneousMarkovChainPtr(new    InhomogeneousMarkovChain());
    model->setPositionSpecificDistribution(positional_distribution);
    model->phased(0);
    model->setAlphabet(alphabet);
    loglikelihood = 0.0;

    for(int j = 0; j < (int)sample_set.size(); j++)
        {
        sample_size += (sample_set[j]->getSequence()).size();
        double v = model->evaluate((sample_set[j]->getSequence()), 0, (sample_set[j]->getSequence()).size()-1);
        if (v <= -HUGE)
            continue;

        loglikelihood += v;
      }
    return model;

  }
  ProbabilisticModelPtr TrainWeightArrayModel::create( ProbabilisticModelParameters & parameters) const
  {
    int size;
    double loglikelihood;
    return create(parameters, loglikelihood, size);
  }
}
