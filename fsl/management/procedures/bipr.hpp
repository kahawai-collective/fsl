#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

class BIPR : public DynamicControlProcedure<> {
public:

    double* const index;

    typedef Fsl::Math::Series::Filters::Ema Smoother;
    Smoother smoother;
    
    double multiplier;
    
    RestrictProportionalChange changes;
    RestrictValue values;
    
    BIPR(
        double* const control,
        const double& starting, 
        double* const index, 
        const double& responsiveness=1,
        const double& multiplier=1,
        const double& change_min=0,const double& change_max=1,
        const double& value_min=0,const double& value_max=INFINITY):
        DynamicControlProcedure(control,starting),
        index(index),
        smoother(responsiveness),
        multiplier(multiplier),
        changes(change_min,change_max),
        values(value_min,value_max){
        reset();
    }
    
    std::string signature(void){
        return boost::str(boost::format("BIPR(%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %smoother.coefficient
            %multiplier
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    BIPR& reset(void){
        value = starting;
        smoother.reset();

        DynamicControlProcedure::reset();
        return *this;
    }
    
    BIPR& operate(unsigned int time){
        double last = value;
        double current = *index;
        double smooth = smoother.update(current);
        value = smooth * multiplier;
        value = changes.restrict(value,last);
        value = values.restrict(value);

        DynamicControlProcedure::operate(time);
        return *this;
    }

};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
