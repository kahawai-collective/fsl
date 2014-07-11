#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

/**
 * BTAR (Biomass Trajectory Asymmetric Restricted)
 *
 * This management procedure
 */
class BTAR : public ControlProcedure<double> {
public:

    double starting;
    
    // Pointer to biomass index
    double* const index;

    // Smoother for index
    typedef Fsl::Math::Series::Filters::Ema Ema;
    Ema smoother;

    // Definition of trajectory
    double initial;
    double rate;
    double target;
    int step;

    // Asymmetry of response
    double asymmetry;

    // Restrictions on changes and values of control   
    RestrictProportionalChange changes;
    RestrictValue values;
    
    BTAR(double* const control,const double& starting, double* const index, 
        const double& responsiveness=1,
        const double& initial=11, const double& rate=0.01,const double& target=1,
        const double& asymmetry=0,
        const double& change_min=0,const double& change_max=1,
        const double& value_min=0,const double& value_max=INFINITY
    ):
        ControlProcedure(control,starting),
        starting(starting),
        index(index),
        smoother(responsiveness),
        initial(initial),rate(rate),target(target),
        asymmetry(asymmetry),
        changes(change_min,change_max),
        values(value_min,value_max){
        reset();
    }
    
    std::string signature(void){
        return boost::str(boost::format("BTAR(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %smoother.coefficient
            %initial%rate%target
            %asymmetry
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    BTAR& reset(void){
        value = starting;
        smoother.reset();
        step = 0;
        return *this;
    }
    
    BTAR& operate(void){
        // Store current control value
        double last = value;
        // Get biomass index and update smoother
        double current = *index;
        double smooth = smoother.update(current);
        // Calculate trajectory index and deviation from it
        double trajectory;
        if(rate>0) trajectory = std::min(initial+rate*step,target);
        else trajectory = std::max(initial+rate*step,target);
        step++;
        double deviation = smooth/trajectory-1;
        // Adjust for asymmetry
        double adjustment;
        if(asymmetry>0 and deviation<0) adjustment = deviation * (1-asymmetry);
        else if(asymmetry<0 and deviation>0) adjustment = deviation * (asymmetry+1);
        else adjustment = deviation;
        // Apply to control value
        value *= 1 + adjustment;
        // Restrict change
        value = changes.restrict(value,last);
        value = values.restrict(value);
        // Do `ControlProcedure::operate()` to actually change the control
        ControlProcedure::operate();
        return *this;
    }
};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
