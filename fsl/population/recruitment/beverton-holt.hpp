#pragma once

namespace Fsl {
namespace Population {
namespace Recruitment {

/*!
Beverton-Holt stock recruitment relationship

@todo Prevent steepness being set outside of 0.2-1 range
@todo Prevent r0 and s0 being set -ve
*/
class BevertonHolt {
public:

    double r0;
    double s0;
    double steepness;

    double alpha(void) const {
        return 4*r0*steepness/(5*steepness-1);
    }

    double beta(void) const {
        return -(s0*steepness-s0)/(5*steepness-1);
    }

    BevertonHolt& initialise(void){
        return *this;
    }
    
    double stock_virgin(void) const {
        return s0;
    }
    
    double recruits_virgin(void) const {
        return r0;
    }

    double recruits(const double& stock) {
        return 4*steepness*r0*stock/((5*steepness-1)*stock+s0*(1-steepness));
    }

    double operator()(const double& stock) {
        return recruits(stock);
    }

};

}
}
}
