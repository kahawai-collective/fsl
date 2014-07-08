#pragma once

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

class FournierRobustifiedMultivariateNormal /*: public Distribution<FournierRobustifiedMultivariateNormal> */ {
public:

    double proportion;
    double size;

    FournierRobustifiedMultivariateNormal(const double& proportion = NAN, const double& size = NAN):
        proportion(proportion),
        size(size){
    }

    bool valid(void) const {
        return std::isfinite(proportion) and size>0;
    }

    double minimum(void) const {
        return 0;
    }

    double maximum(void) const {
        return 1;
    }

    double mean(void) const {
        return proportion;
    }

    double sd(void) const {
        return proportion * (1-proportion);
    }

    double likelihood(const double& x) const {
        double n = x*size;
        double n_apos = std::min(size,1000.0);
        double e_apos = (1-x)*x+0.1/n;
        return std::exp(-std::pow(proportion-x,2)/(2*e_apos/n_apos)+0.01)-0.5*e_apos;
    }
    
    double pdf(const double& x) const {
        return NAN;
    }

    double quantile(const double& p) const {
        return NAN;
    }

    double random(void) const {
        return NAN;
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(proportion,"proportion")
            .data(size,"size")
        ;
    }
};

}}}
