#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

class BIPR : public ControlProcedure<double> {
public:

    double starting;
    
    double* const index;

    typedef Fsl::Math::Series::Filters::Ema Ema;
    Ema ema;
    
    double multiplier;
    
    RestrictProportionalChange changes;
    RestrictValue values;
    
    BIPR(double* const control,const double& starting, double* const index, const double& responsiveness=1,const double& multiplier=1,const double& change_min=0,const double& change_max=1,const double& value_min=0,const double& value_max=INFINITY):
        ControlProcedure(control,starting),
        starting(starting),
        index(index),
        ema(responsiveness),
        multiplier(multiplier),
        changes(change_min,change_max),
        values(value_min,value_max){
        reset();
    }
    
    std::string signature(void){
        return boost::str(boost::format("BIPR(%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %ema.coefficient
            %multiplier
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    BIPR& reset(void){
        value = starting;
        ema.reset();
        return *this;
    }
    
    BIPR& operate(void){
        double last = value;
        double current = *index;
        double smooth = ema.update(current);
        value = smooth * multiplier;
        value = changes.restrict(value,last);
        value = values.restrict(value);

        ControlProcedure::operate();
        return *this;
    }

};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
