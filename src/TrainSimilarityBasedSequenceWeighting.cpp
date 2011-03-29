#include "ProbabilisticModel.hpp"
#include "SimilarityBasedSequenceWeighting.hpp"
#include "TrainSimilarityBasedSequenceWeighting.hpp"
#include "ConfigurationReader.hpp"
#include "ContextTree.hpp"
#include "VariableLengthMarkovChain.hpp"
#include "Symbol.hpp"
#include "SequenceFactory.hpp"
#include "util.hpp"
namespace tops {

  double test (ProbabilisticModelPtr m, std::string q, int max)
  {
    if(q.size() >= max)
      {
        SequenceFactory f(m->alphabet());
        std::stringstream s;
        for (int i = 0; i < q.size(); i++)
          {
            if(i == 0)
              s << q[i];
            else
              s << " " << q[i];
          }
        Sequence x = f.createSequence(s.str());
        double p = exp(m->evaluate(x, 0, x.size()-1));
        std::cerr << q << " " << p  << std::endl;
        return p;
      }
    else
      {
        double sum = 0.0;
        for(int i = 0 ; i < m->alphabet()->size(); i++)
          {
            std::stringstream s;
            s <<q<< m->alphabet()->getSymbol(i)->name() ;
            sum += test(m, s.str(), max);
          }
        return sum;
      }
  }

  double calculate_normalizer (int skip_length,int skip_offset, int max_length, std::map<std::string, double > & counter, int alphabet_size)
  {
    int npatterns_differ_1 = 0;
    npatterns_differ_1 = (alphabet_size - 1) * (max_length - skip_length);
    if(skip_length < 0)
      npatterns_differ_1 = (alphabet_size - 1) * (max_length );
    double sum = 0.0;
    std::map<std::string, double>::const_iterator it;
    std::map<std::string, double>::const_iterator it2;
    for(it = counter.begin(); it != counter.end(); it++)
      {
        sum += it->second;

        int diff = 0;
        int np_differ_1  = 0;

        for(it2 = counter.begin(); it2 != counter.end(); it2++)
          {
            std::string a = it->first;
            std::string b = it2->first;
            for(int i = 0; i < max_length; i++)
              if((i >= skip_offset) && (i <= skip_offset+skip_length)){
                if(a[i] != b[i])
                  diff+=2;
              }else if(a[i] != b[i])
                diff++;

            if(diff == 1) {
              sum += it2->second * 0.001;
              np_differ_1 ++;
            }
          }
        sum += 0.001*it->second*(npatterns_differ_1 - np_differ_1);
      }
    return sum;
  }






ProbabilisticModelPtr TrainSimilarityBasedSequenceWeighting::create(
                ProbabilisticModelParameters & parameters, double & loglikelihood,
                int & sample_size) const {
        ProbabilisticModelParameterValuePtr trainpar =
                        parameters.getMandatoryParameterValue("training_set");
        ProbabilisticModelParameterValuePtr alphapar =
                        parameters.getMandatoryParameterValue("alphabet");

        ProbabilisticModelParameterValuePtr offsetpar = parameters.getOptionalParameterValue("skip_offset");
        ProbabilisticModelParameterValuePtr skiplengthpar = parameters.getOptionalParameterValue("skip_length");
        ProbabilisticModelParameterValuePtr skipseqpar = parameters.getOptionalParameterValue("skip_sequence");

        if ((trainpar == NULL) || (alphapar == NULL) ) {
          std::cerr << help() << std::endl;
          exit(-1);
        }
        int skip_length = -1;
        int skip_offset  = -1;
        std::string skip_seq;
        if((offsetpar != NULL) && (skiplengthpar != NULL))
          {
            skip_offset = offsetpar->getInt();
            skip_length = skiplengthpar->getInt();
            skip_seq = skiplengthpar -> getString();

          }

        AlphabetPtr alphabet = AlphabetPtr(new Alphabet());
        alphabet ->initializeFromVector(alphapar->getStringVector());
        SequenceEntryList sample_set;
        readSequencesFromFile(sample_set, alphabet, trainpar->getString());
        std::map<std::string, double> counter;
        int min_length = 999999999;
        for(int i = 0; i < (int) sample_set.size(); i++)
          {
            std::stringstream o;
            for(int j = 0; j < (int)(sample_set[i]->getSequence()).size();j++)
              {
                o << alphabet->getSymbol((sample_set[i]->getSequence())[j])->name();
              }
            if(counter.find(o.str()) == counter.end())
              {
                counter[o.str()] = 1;
              }
            else
              {
                counter[o.str()] += 1;
              }
            if((int)o.str().size() < min_length)
              min_length = o.str().size();
          }
        std::string q;
        double normalize = calculate_normalizer(skip_length,skip_offset,  min_length, counter, alphabet->size());

        ProbabilisticModelParameters pars;
        pars.add("alphabet", alphapar);
        pars.add("counter", DoubleMapParameterValuePtr(new DoubleMapParameterValue(counter)));
        pars.add("normalizer", DoubleParameterValuePtr(new DoubleParameterValue(normalize)));
        if(skiplengthpar != NULL && offsetpar != NULL) {
          pars.add("skip_length", skiplengthpar);
          pars.add("skip_offset", offsetpar);
          pars.add("skip_sequence", skipseqpar);
        }
        ProbabilisticModelPtr m = SimilarityBasedSequenceWeightingPtr(new SimilarityBasedSequenceWeighting());
        m->initialize(pars);

        //      std::string x;
        //      std::cerr << "SUM: " << test(m, x, min_length)<< std::endl;

        loglikelihood = 0.0;
        sample_size = 0;
        for (int i = 0; i < (int) sample_set.size(); i++) {
          loglikelihood
            += m->evaluate((sample_set[i]->getSequence()), 0, (sample_set[i]->getSequence()).size() - 1);
          sample_size += (sample_set[i]->getSequence()).size();
        }
        return m;
}
  ProbabilisticModelPtr TrainSimilarityBasedSequenceWeighting::create(
                                                                      ProbabilisticModelParameters & parameters) const {
    double loglike;
    int samplesize;
    return create(parameters, loglike, samplesize);

  }

}
;
