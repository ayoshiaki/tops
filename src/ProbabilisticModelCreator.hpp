#ifndef PROBABILISTIC_MODEL_CREATOR_HPP
#define PROBABILISTIC_MODEL_CREATOR_HPP

#include "ProbabilisticModel.hpp"
#include "ConfigurationReader.hpp"


namespace tops {

  //! Creates a probabilistic model
  class ProbabilisticModelCreator {
  public:
    ProbabilisticModelCreator () {}
    virtual ~ProbabilisticModelCreator () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const {
      ProbabilisticModelPtr n;
      not_implemented( "create ()");
      return n;
    }
    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, double & loglikelihood, int & sample_size) const {
      ProbabilisticModelPtr n;
      not_implemented( "create (parameters, loglikelihood)");
      return n;
    }

    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::vector<std::string> & sample_set, double & loglikelihood, int & sample_size) const {
      ProbabilisticModelPtr n;
      not_implemented( "create (parameters, loglikelihood)");
      return n;
    }

    //! Creates the model and returns the loglikelihood of the training set
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters, const std::map<std::string,ProbabilisticModelPtr> & models) const {
      return create(parameters);
    }


    //! Provides a help
    virtual std::string help() const {
      std::string s;
      not_implemented( "help()");
      return s;
    }
    virtual void not_implemented( std::string method) const {
      std::cerr << "ERROR: Not implemented: " << method << " !" << std::endl;
      exit(-1);
    }
    virtual void setCreator(ProbabilisticModelCreatorPtr creator)  {
      not_implemented("setCreator()");
      exit(-1);
    }


  };
  typedef boost::shared_ptr <ProbabilisticModelCreator> ProbabilisticModelCreatorPtr ;
};


#endif
