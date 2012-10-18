#pragma once

#include <boost/math/distributions/lognormal.hpp>
#include <boost/random/lognormal_distribution.hpp>

#include <figo/math/probability/distribution.hpp>

namespace Figo {
namespace Math {
namespace Probability {

using namespace Figo;

class Lognormal : public Distribution<Lognormal> {
private:
    double mean_;
    double sd_;
    
public:
    
    Lognormal(const double& mean=1, const double& sd=1):
        mean_(mean),
        sd_(sd){
    }
    
    double mean(void) const {
        return mean_;
    }

    boost::math::lognormal boost_dist(void) const {
        //boost::math uses different properties for location and scale so some conversion is necessary
        double scale_squared = std::log(1.0+sd_*sd_/(mean_*mean_));
        double location = std::log(mean_)-0.5*scale_squared;
        double scale = std::sqrt(scale_squared);
        return boost::math::lognormal(location,scale);
    }
    
    double random(void) const {
        boost::lognormal_distribution<> distr(mean_,sd_);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }
};

}}}
