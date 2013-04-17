#pragma once

#include <boost/math/distributions/normal.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/format.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Normal : public Distribution<Normal> {
private:

    double mean_;
    double sd_;
    
public:
    
    Normal(const double& mean=1, const double& sd=1):
        mean_(mean),
        sd_(sd){
    }
    
    std::string describe(void) const {
        return str(boost::format("Normal(%g,%g)")%mean()%sd());
    }
    
    double mean(void) const {
        return mean_;
    }

    Normal& mean(const double& mean) {
        mean_ = mean;
		return *this;
    }
	
    double sd(void) const {
        return sd_;
    }

    Normal& sd(const double& sd) {
        sd_ = sd;
		return *this;
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
