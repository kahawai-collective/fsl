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
public:
    double mean;
    double sd;

    Normal(const double& mean_=1, const double& sd_=1):
        mean(mean_),
        sd(sd_){
    }
    
    std::string describe(void) const {
        return str(boost::format("Normal(%g,%g)")%mean%sd);
    }

    boost::math::normal boost_dist(void) const {
        return boost::math::normal(mean,sd);
    }
    
    double random(void) const {
        boost::normal_distribution<> distr(mean,sd);
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
