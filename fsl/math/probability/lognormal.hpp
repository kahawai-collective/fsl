#pragma once

#include <boost/math/distributions/lognormal.hpp>
#include <boost/random/lognormal_distribution.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Lognormal : public Distribution<Lognormal> {
public:

    double location;
    double dispersion;

    Lognormal(const double& location = NAN, const double& dispersion = NAN):
        location(location),
        dispersion(dispersion){
    }

    bool valid(void) const {
        return location>0 and dispersion>0;
    }

    double minimum(void) const {
        return 0;
    }

    double mean(void) const {
        return location;
    }

    Lognormal& mean(const double& mean) {
        location = mean;
        return *this;
    }

    double sd(void) const {
        return dispersion;
    }

    Lognormal& sd(const double& sd) {
        dispersion = sd;
        return *this;
    }

    boost::math::lognormal boost_dist(void) const {
        return boost::math::lognormal(location,dispersion);
    }
    
    double random(void) const {
        boost::lognormal_distribution<> distr(location,dispersion);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(location,"location")
            .data(dispersion,"dispersion")
        ;
    }
};

}}}
