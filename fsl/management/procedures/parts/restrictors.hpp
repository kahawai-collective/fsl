#pragma once

namespace Fsl {
namespace Management {

class RestrictValue {
public:
    double lower;
    double upper;

    RestrictValue(const double& lower=0,const double& upper=INFINITY):
        lower(lower),
        upper(upper){
    }

    double restrict(const double& value){
        return (value<lower)?(lower):(value>upper?upper:value);
    }
};

class RestrictValuePeriods : public RestrictValue {
public:
    struct Period : RestrictValue {
        uint start = 0;
        uint finish = 0;
    };
    Period periods[10];

    double restrict(const double& value, uint time){
        for(auto period : periods){
            if(period.start==0 and period.finish==0) break;
            if(time>=period.start and time<=period.finish){
                return period.restrict(value);
            }
        }
        return RestrictValue::restrict(value);
    }
};

class RestrictProportionalChange {
public:
    double lower;
    double upper;

    RestrictProportionalChange(const double& lower=0,const double& upper=INFINITY):
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