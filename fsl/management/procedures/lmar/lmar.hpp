#pragma once

#include <fsl/management/procedure.hpp>
#include <fsl/management/procedures/parts/restrictors.hpp>
#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {


/**
 * Length Moving Average Range
 */
class LMAR : public DynamicControlProcedure<> {
public:

    double* index;

    typedef Fsl::Math::Series::Filters::Ema Smoother;
    Smoother smoother;

    double target;
    double buffer;
    double reaction = 1;
   
    uint wait;
    uint time_last = 0;

    double max = 1e50;

public:

    LMAR(double* control, double starting):
        DynamicControlProcedure<>(control, starting){}
    
    virtual std::string signature(void) const {
        return boost::str(boost::format("LMAR\t%s\t%s\t%s\t%s\t%s\t%s\t0\t0\t0\t0")
            %starting
            %smoother.coefficient
            %target
            %buffer
            %reaction
            %wait
        );
    }
    
    virtual void reset(void){
        DynamicControlProcedure::reset();
        smoother.reset();
        value = starting;
        time_last = 0;
    }
    
    virtual void operate(uint time){
        //double last = value;
        double current = *index;
        double smooth = smoother.update(current);
        if (time_last == 0 or time-time_last >= wait) {
            if (smooth < target - buffer) {
                value *= std::pow(smooth/target, reaction);
                time_last = time;
            } else if (smooth > target + buffer) {
                value *= std::pow(smooth/target, reaction);
                time_last = time;
            }
        }
        if (value > max) value = max;
        // Do `DynamicControlProcedure::operate()` to actually change the control
        DynamicControlProcedure::operate(time);
    }

    virtual std::string record(uint time) const {
        if (time == 0) return "time\tindex\tsmooth\ttarget\tbuffer\tvalue\n";
        else return boost::str(
            boost::format("%s\t%s\t%s\t%s\t%s\t%s\n") % 
                time % *index % smoother.value % target % buffer % value
        );
    }

};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
