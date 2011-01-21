#include "ContextTree.hpp"
#include "TrainWeightArrayModel.hpp"
#include "InhomogeneousMarkovChain.hpp"
#include "ProbabilisticModelParameter.hpp"
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
            if(fixseq) {
              Sequence s;
              s.resize(o+1);
              int l = 0;
              int m = i - o + offset;
              if(m < 0) m = 0;
              while( (m < (int) (sample_set[j]->getSequence()).size()) && (l <= o))
                {
                  s[l] = (sample_set[j]->getSequence())[m];
                  if(fixseq && (fixed_pos <= m) && (((int)fixed.size() - (m - (int)fixed_pos +1))>= 0)){
                    int p = m-fixed_pos;
                    s[l] = fixed[p];
                  }
                  l++; m++;
                }
              SequenceEntryPtr entry = SequenceEntryPtr(new SequenceEntry(alphabet));
              entry->setSequence(s);
              positionalSample.push_back(entry);
            } else {
              for(int k = -vicinity; k <= vicinity; k++)
                {
                  Sequence s;
                  s.resize(o+1);
                  int l = 0;
                  int m = i - o + k + offset;

                  if(m < 0)
                      continue;
                  if((m + o) >= (sample_set[j]->getSequence()).size())
                      continue;
                  while( (m < (int) (sample_set[j]->getSequence()).size()) && (l <= o))
                    {
                      s[l] = (sample_set[j]->getSequence())[m];
                      if(fixseq && (fixed_pos <= m) && (((int)fixed.size() - (m - (int)fixed_pos +1))>= 0)){
                        int p = m-fixed_pos;
                        s[l] = fixed[p];
                      }
                      l++; m++;
                    }
                  SequenceEntryPtr entry = SequenceEntryPtr(new SequenceEntry(alphabet));
                  entry->setSequence(s);
                  positionalSample.push_back(entry);
                }
            }
          }
        if(fixseq && (fixed_pos <= i) && (((int)fixed.size() - (i - (int)fixed_pos +1))>= 0)){
          ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
          tree->initializeCounter(positionalSample, o, 0);
          tree->normalize();
          positional_distribution[i] = tree;
        } else {
          ContextTreePtr tree = ContextTreePtr(new ContextTree(alphabet));
          tree->initializeCounter(positionalSample, o, pseudocounts);
          tree->normalize();
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
        loglikelihood += model->evaluate((sample_set[j]->getSequence()), 0, (sample_set[j]->getSequence()).size()-1);
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
