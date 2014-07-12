#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

/**
 * Trajectory Status Assymetric Restricted
 */
class TSAR : public DynamicControlProcedure<> {
public:

    double* const index;

    typedef Fsl::Math::Series::Filters::Ema Smoother;
    Smoother smoother;
    
    // Definition of trajectory
    double initial;
    double rate;
    double target;
    int step;

    double trigger;
    
    RestrictProportionalChange changes;
    RestrictValue values;

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

    double status_last_;

    double multiplier_;

public:
    
    TSAR(
        double* const control,
        const double& starting, 
        double* const index, 
        const double& responsiveness=1,
        const double& initial=1, const double& rate=0.01,const double& target=2,
        const double& trigger=1,
        const double& change_min=0,const double& change_max=1,
        const double& value_min=0,const double& value_max=INFINITY):
        DynamicControlProcedure(control,starting),
        index(index),
        smoother(responsiveness),
        initial(initial),rate(rate),target(target),
        trigger(trigger),
        changes(change_min,change_max),
        values(value_min,value_max){
        reset();
    }
    
    std::string signature(void){
        return boost::str(boost::format("TSAR(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)")
            %starting
            %smoother.coefficient
            %initial%rate%target
            %trigger
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
        // In the first step do some intitialisation
        if(step==0){
            initial_value_ = smooth * initial;
            rate_value_ = smooth * rate;
            target_value_ = smooth * target;
            multiplier_ = value*1/initial;
        }
        // Calculate trajectory index and status relative to it
        double trajectory;
        if(rate>0) trajectory = std::min(initial_value_+rate_value_*step,target_value_);
        else trajectory = std::max(initial_value_+rate_value_*step,target_value_);
        double status = smooth/trajectory;
        if(step==0){
            status_last_ = status;
        }
        // If greater than x% change in since last time a change was made
        // then adjust accordingly
        double change = std::log(status/status_last_);
        if(std::fabs(change)>=trigger){
            value = status * multiplier_;
        }
        // Restrict change and range of values
        value = changes.restrict(value,last);
        value = values.restrict(value);
        // Now check to see if there has been a change
        if(value!=last){
            status_last_ = status;
        }
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
