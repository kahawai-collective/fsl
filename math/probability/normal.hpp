#pragma once

#include <boost/math/distributions/normal.hpp>
#include <boost/random/normal_distribution.hpp>

#include <figo/math/probability/distribution.hpp>

namespace Figo {
namespace Math {
namespace Probability {

using namespace Figo;

class Normal : public Distribution<Normal> {
private:

    double mean_;
    double sd_;
    
public:
    
    Normal(const double& mean=1, const double& sd=1):
        mean_(mean),
        sd_(sd){
    }
    
    double mean(void) const {
        return mean_;
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

}}}
