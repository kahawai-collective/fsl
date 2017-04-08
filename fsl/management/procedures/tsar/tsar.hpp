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

    double* index;

    typedef Fsl::Math::Series::Filters::Ema Smoother;
    Smoother smoother;
    
    // Definition of trajectory
    double initial;
    double slope;
    double target;

    /**
     * Start point for trajectory
     */
    uint start = 0;

    /**
     * Asymmetry of response
     */
    double asymmetry;
    
    RestrictProportionalChange changes;
    RestrictValuePeriods values;

    int step;
    double multiplier;

public:
    
    TSAR(
        double* const control,
        const double& starting, 
        double* const index, 
        const double& responsiveness=1,
        const double& initial=1, const double& slope=0.01,const double& target=2, const uint& start=0,
        const double& asymmetry=1):
        DynamicControlProcedure(control,starting),
        index(index),
        smoother(responsiveness),
        initial(initial),slope(slope),target(target),start(start),
        asymmetry(asymmetry){
        reset();
    }
    
    std::string signature(void) const {
        return boost::str(boost::format("TSAR,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s")
            %starting
            %smoother.coefficient
            %initial%slope%target%start
            %asymmetry
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    void reset(void){
        smoother.reset();
        step = 0;
        DynamicControlProcedure::reset();
    }
    
    void operate(uint time){
        double last = value;
        double current = *index;
        double smooth = smoother.update(current);
        // Calculate trajectory
        double trajectory = initial+slope*(start+step);
        if(slope>0) trajectory = std::min(trajectory,target);
        else trajectory = std::max(trajectory,target);
        // Calculate status relative to trajectory
        double status = smooth/trajectory;
        // Calculate assymetric response
        double response;
        double log_status = std::log(status);
        if(asymmetry>0 and log_status>0) response = std::exp(log_status*asymmetry);
        else if(asymmetry<0 and log_status<0) response = std::exp(log_status*std::fabs(asymmetry));
        else response = status;
        // In the first time step the multiplier is set so that the procedures
        // results in the current control value if on trajectory
        if(step==0) multiplier = value;
        // Calculate control value using multiplier
        value = multiplier * response;
        // Restrict change and range of values
        value = changes.restrict(value,last);
        value = values.restrict(value,time);
        // Do `DynamicControlProcedure::operate()` to actually change the control
        DynamicControlProcedure::operate(time);
        // Increment step
        step++;
    }

};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
