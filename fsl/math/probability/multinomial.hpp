#pragma once

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

class Multinomial {
public:

    double proportion;
    double size;

    Multinomial(const double& proportion = NAN, const double& size = NAN):
        proportion(proportion),
        size(size){
    }

    bool valid(void) const {
        return size>0;
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

    double pdf(const double& x) const {
        return 0;
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
