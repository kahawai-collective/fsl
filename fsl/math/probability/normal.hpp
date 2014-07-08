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

    Normal(const double& mean = NAN, const double& sd = NAN):
        mean_(mean),
        sd_(sd){
    }
    
    bool valid(void) const {
        return std::isfinite(mean_) and sd_>0;
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

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(mean_,"mean")
            .data(sd_,"sd")
        ;
    }
};

class NormalCv : public Normal {
public:
    NormalCv(const double& mean=1, const double& cv=1):
        Normal(mean,std::fabs(mean*cv)){
    }
};

}}}
