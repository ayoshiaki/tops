#ifndef SMOOTHED_HISTOGRAM_STANKE_HPP
#define SMOOTHED_HISTOGRAM_STANKE_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops 
{

  //! Use this to create a smoothed histogram
  class SmoothedHistogramStanke:  public ProbabilisticModelCreator {
  public:
    SmoothedHistogramStanke () {}
    virtual ~SmoothedHistogramStanke () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const;

    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }
    virtual std::string factory_name() const {
      return "SmoothedHistogramStanke";
    }

  };
  typedef boost::shared_ptr <SmoothedHistogramStanke> SmoothedHistogramStankePtr ;


}

#endif
