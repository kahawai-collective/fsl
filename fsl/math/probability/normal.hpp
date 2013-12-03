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
    
    const double& mean(void) const {
        return mean_;
    }

    double& mean(void) {
        return mean_;
    }

    const double& sd(void) const {
        return sd_;
    }

    double& sd(void) {
        return sd_;
    }

    boost::math::normal boost_dist(void) const {
        return boost::math::normal(mean(),sd());
    }
    
    double random(void) const {
        boost::normal_distribution<> distr(mean(),sd());
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }
};

class NormalCv : public Normal {
public:
    NormalCv(const double& mean=1, const double& cv=1):
        Normal(mean,std::fabs(mean*cv)){
    }
};

}}}
