#pragma once

namespace Fsl {
namespace Population {
namespace Recruitment {

/**
Beverton-Holt stock recruitment relationship parameterised using steepness and
pristine recruitment
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

    double operator()(const double& stock) {
        return 4*steepness*r0*stock/((5*steepness-1)*stock+s0*(1-steepness));
    }

};

}
}
}
