#ifndef SMOOTHED_HISTOGRAM_KERNEL_DENSITY_HPP
#define SMOOTHED_HISTOGRAM_KERNEL_DENSITY_HPP
#include "ProbabilisticModelCreator.hpp"

namespace tops 
{

  //! Estimates a smoothed histogram using kernel density estimation
  class SmoothedHistogramKernelDensity:  public ProbabilisticModelCreator {
  public:
    SmoothedHistogramKernelDensity () {}
    virtual ~SmoothedHistogramKernelDensity () {};
    //! Creates a probability model 
    /*! \param parameters is a set of parameters that is utilized to build the model */
    virtual ProbabilisticModelPtr create( ProbabilisticModelParameters & parameters) const;

    //! Provides a help
    virtual std::string help() const {
      std::string s;
      return s;
    }
    virtual std::string factory_name() const {
      return "SmoothedHistogramKernelDensity";
    }

  };
  typedef boost::shared_ptr <SmoothedHistogramKernelDensity> SmoothedHistogramKernelDensityPtr ;


}

#endif
