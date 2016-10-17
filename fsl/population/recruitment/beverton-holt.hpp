#pragma once

#include <boost/format.hpp>

#include <stencila/structure.hpp>
using Stencila::Structure;

namespace Fsl {
namespace Population {
namespace Recruitment {

/**
Beverton-Holt stock recruitment relationship parameterised using steepness and
pristine stock and recruitment levels
*/
class BevertonHolt : public Structure<BevertonHolt> {
public:

    double r0;
    double s0;
    double h;

    void check(void) const {
        if(not std::isfinite(r0) or r0 <= 0){
            throw std::runtime_error(str(boost::format("`BevertonHolt::r0` has invalid value.\n  r0: %s")%r0));
        }
        if(not std::isfinite(s0) or s0 <= 0){
            throw std::runtime_error(str(boost::format("`BevertonHolt::s0` has invalid value.\n s0: %s")%s0));
        }
        if(not std::isfinite(h) or h<=0.2 or h>1){
            throw std::runtime_error(str(boost::format("`BevertonHolt::h` has invalid value.\n h: %s")%h));
        }
    }

    double alpha(void) const {
        return 4*r0*h/(5*h-1);
    }

    double beta(void) const {
        return -(s0*h-s0)/(5*h-1);
    }

    double operator()(const double& stock) {
        return 4*h*r0*stock/((5*h-1)*stock+s0*(1-h));
    }

    template<class Mirror>
    void reflect(Mirror& mirror){
        mirror
            .data(r0,"r0")
            .data(s0,"s0")
            .data(h,"h")
        ;
    }
};

}
}
}
