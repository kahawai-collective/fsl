#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

/**
 * Trajectory Status Assymetric Restricted (TSAR)
 */
class TSAR : public DynamicControlProcedure<> {
public:

    double* const index;

    typedef Fsl::Math::Series::Filters::Ema Smoother;
    Smoother smoother;
    
    // Definition of trajectory
    double initial;
    double slope;
    double target;
    int step;

    double asymmetry;
    
    RestrictProportionalChange changes;
    RestrictValue values;

    double multiplier;

public:
    
    TSAR(
        double* const control,
        const double& starting, 
        double* const index, 
        const double& responsiveness=1,
        const double& initial=1, const double& slope=0.01,const double& target=2,
        const double& asymmetry=1,
        const double& change_min=0,const double& change_max=1,
        const double& value_min=0,const double& value_max=INFINITY):
        DynamicControlProcedure(control,starting),
        index(index),
        smoother(responsiveness),
        initial(initial),slope(slope),target(target),
        asymmetry(asymmetry),
        changes(change_min,change_max),
        values(value_min,value_max){
        reset();
    }
    
    std::string signature(void){
        return boost::str(boost::format("TSAR(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %smoother.coefficient
            %initial%slope%target
            %asymmetry
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    TSAR& reset(void){
        smoother.reset();
        step = 0;
        DynamicControlProcedure::reset();
        return *this;
    }
    
    TSAR& operate(void){
        double last = value;
        double current = *index;
        double smooth = smoother.update(current);
        // Calculate trajectory
        double trajectory;
        if(slope>0) trajectory = std::min(initial+slope*step,target);
        else trajectory = std::max(initial+slope*step,target);
        // Calculate status relative to trajectory
        double status = smooth/trajectory;
        // Calculate assymetric response
        double response;
        double log_status = std::log(status);
        if(asymmetry>0 and log_status>0) response = std::exp(log_status*asymmetry);
        else if(asymmetry<0 and log_status<0) response = std::exp(log_status*std::fabs(asymmetry));
        else response = status;
        // In the first time step the multiplier is set so that the procedures
        // results in the current control value in the first step. ie. their will be no change in the the control (e.g. TAC)
        // This is intentional, as the procedures is intended to change the control gradually
        // in response to status relative to the trajectory.
        if(step==0) multiplier = value/response;
        // Calculate control value using multiplier
        value = response * multiplier;
        // Restrict change and range of values
        value = changes.restrict(value,last);
        value = values.restrict(value);
        // Do `DynamicControlProcedure::operate()` to actually change the control
        DynamicControlProcedure::operate();
        // Increment step
        step++;
        return *this;
    }

};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
