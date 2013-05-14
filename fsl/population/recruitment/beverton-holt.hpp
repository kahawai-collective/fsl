#pragma once

#include <fsl/property.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {

/*!
Beverton-Holt stock recruitment relationship

@todo Prevent steepness being set outside of 0.2-1 range
@todo Prevent r0 and s0 being set -ve
*/
class BevertonHolt {

    FSL_PROPERTY(BevertonHolt,s0,double)
    FSL_PROPERTY(BevertonHolt,r0,double)
    FSL_PROPERTY(BevertonHolt,steepness,double)

public:

    double alpha(void) const {
        return 4*r0_*steepness_/(5*steepness_-1);
    }

    double beta(void) const {
        return -(s0_*steepness_-s0_)/(5*steepness_-1);
    }

    BevertonHolt& initialise(void){
        return *this;
    }

    double operator()(const double& stock) {
        return 4*steepness_*r0_*stock/((5*steepness_-1)*stock+s0_*(1-steepness_));
    }

};

}
}
}
