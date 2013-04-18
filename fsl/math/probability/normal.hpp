#pragma once

#include <boost/math/distributions/normal.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/format.hpp>

#include <fsl/property.hpp>
#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Normal : public Distribution<Normal> {

    FSL_PROPERTY(Normal,mean,double)
    FSL_PROPERTY(Normal,sd,double)

public:

    Normal(const double& mean=1, const double& sd=1):
        mean_(mean),
        sd_(sd){
    }
    
    std::string describe(void) const {
        return str(boost::format("Normal(%g,%g)")%mean()%sd());
    }

    boost::math::normal boost_dist(void) const {
        return boost::math::normal(mean_,sd_);
    }
    
    double random(void) const {
        boost::normal_distribution<> distr(mean_,sd_);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }
};

class NormalCv : public Normal {
public:
	NormalCv(const double& mean, const double& cv):
		Normal(mean,std::fabs(mean*cv)){
	}
};

}}}
