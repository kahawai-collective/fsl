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
class BTAR : public BIPR {
public:

    // Definition of trajectory
    double initial;
    double rate;
    double target;
    int step;

    //
    double moderation;

    // Asymmetry of response
    double asymmetry;

private:

    /**
     * Initial value of trajectory.
     */
    double initial_value_;

    /**
     * Target value of trajectory.
     */
    double rate_value_;

    /**
     * Target value of trajectory.
     */
    double target_value_;

public:
    
    BTAR(
        double* const control,
        const double& starting,
        double* const index, 
        const double& responsiveness=1,
        const double& multiplier=1,
        const double& initial=1, const double& rate=0.01,const double& target=2,
        const double& moderation=1,
        const double& asymmetry=0,
        const double& change_min=0,const double& change_max=1,
        const double& value_min=0,const double& value_max=INFINITY
    ):
        BIPR(control,starting,index,responsiveness,multiplier,change_min,change_max,value_min,value_max),
        initial(initial),rate(rate),target(target),
        moderation(moderation),
        asymmetry(asymmetry){
    }
    
    std::string signature(void){
        return boost::str(boost::format("BTAR(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %smoother.coefficient
            %multiplier
            %initial%rate%target
            %moderation
            %asymmetry
            %changes.lower%changes.upper
            %values.lower%values.upper
        );
    }
    
    BTAR& reset(void){
        step = 0;
        BIPR::reset();
        return *this;
    }
    
    BTAR& operate(uint time){
        // Store current control value
        double last = value;
        // Get biomass index and update smoother
        double current = *index;
        double smooth = smoother.update(current);
        // In the first step do some intitialisation
        if(step==0){
            initial_value_ = smooth * initial;
            rate_value_ = smooth * rate;
            target_value_ = smooth * target;
            //multiplier_ = value/initial_value_;
        }
        // Calculate trajectory index and deviation from it
        double trajectory;
        if(rate>0) trajectory = std::min(initial_value_+rate_value_*step,target_value_);
        else trajectory = std::max(initial_value_+rate_value_*step,target_value_);
        double deviation = smooth/trajectory-1;
        // Calculate status relative to trajectory including
        // adjusting for any asymmetry
        double asymmetric;
        if(asymmetry>0 and deviation<0) asymmetric = deviation * (1-asymmetry);
        else if(asymmetry<0 and deviation>0) asymmetric = deviation * (asymmetry+1);
        else asymmetric = deviation;
        //
        double moderating;
        if(moderation==0) moderating = 1;
        else{
            if(asymmetric<0) moderating = 1-std::pow(std::fabs(asymmetric),1/moderation);
            else moderating = 1+std::pow(asymmetric,1/moderation);
        }

        // Calculate a new control value
        value = smooth * multiplier * moderating;
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
