#ifndef SMOOTHED_HISTOGRAM_BURGE_HPP
#define SMOOTHED_HISTOGRAM_BURGE_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops 
{


  class SmoothedHistogramBurge:  public ProbabilisticModelCreator {
  public:
    SmoothedHistogramBurge () {}
    virtual ~SmoothedHistogramBurge () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const;

    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }
    virtual std::string factory_name() const {
      return "SmoothedHistogramBurge";
    }

  };
  typedef boost::shared_ptr <SmoothedHistogramBurge> SmoothedHistogramBurgePtr ;


}

#endif
