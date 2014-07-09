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

    void check(void) const {
        if(not std::isfinite(r0) or r0 <= 0) throw std::runtime_error(str(boost::format("`BevertonHolt::r0` has invalid value <%s>")%r0));
        if(not std::isfinite(s0) or s0 <= 0) throw std::runtime_error(str(boost::format("`BevertonHolt::s0` has invalid value <%s>")%s0));
        if(not std::isfinite(steepness) or steepness<=0.2 or steepness>1) throw std::runtime_error(str(boost::format("`BevertonHolt::steepness` has invalid value <%s>")%steepness));
    }

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
