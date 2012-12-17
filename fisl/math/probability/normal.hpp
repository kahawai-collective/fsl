#pragma once

#include <boost/math/distributions/normal.hpp>
#include <boost/random/normal_distribution.hpp>

#include <fisl/math/probability/distribution.hpp>

namespace Fisl {
namespace Math {
namespace Probability {

using namespace Fisl;

class Normal : public Distribution<Normal> {
private:

    double mean_;
    double sd_;
    
public:
    
    Normal(const double& mean=1, const double& sd=1):
        mean_(mean),
        sd_(sd){
    }
	
	Normal(const double& mean, const double& cv, bool is_cv):
        mean_(mean),
        sd_(std::fabs(mean*cv)){
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
