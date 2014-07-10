#pragma once

namespace Fsl {
namespace Management {

struct RestrictValue {
    double lower;
    double upper;

    RestrictValue(const double& lower,const double& upper):
        lower(lower),
        upper(upper){
    }

    double restrict(const double& value){
        return (value<lower)?(lower):(value>upper?upper:value);
    }
};

struct RestrictProportionalChange {
    double lower;
    double upper;

    RestrictProportionalChange(const double& lower,const double& upper):
        lower(lower),
        upper(upper){
    }

    double restrict(const double& value, const double& last){
        auto change = value/last-1;
        auto delta = std::fabs(change);
        auto sign = change/delta;
        auto change_restricted =  (delta<lower)?0:(delta>upper?(sign*upper):change);
        return last * (1+change_restricted);
    }
};

} // namespace Management
} // namespace Fsl