#pragma once

#include <fsl/math/series/filters/ema.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {
    
typedef double Real;
typedef std::string String;
    
struct RestrictValue {
    Real lower;
    Real upper;

    RestrictValue(const Real& lower,const Real& upper):
        lower(lower),
        upper(upper){
    }

    Real restrict(const Real& value){
        return (value<lower)?(lower):(value>upper?upper:value);
    }
};

struct RestrictProportionalChange {
    Real lower;
    Real upper;

    RestrictProportionalChange(const Real& lower,const Real& upper):
        lower(lower),
        upper(upper){
    }

    Real restrict(const Real& value, const Real& last){
        auto change = value/last-1;
        auto delta = std::fabs(change);
        auto sign = change/delta;
        auto change_restricted =  (delta<lower)?0:(delta>upper?(sign*upper):change);
        return last * (1+change_restricted);
    }
};

class Procedure {

    void reset(void){
    }

    void operate(){
    }
};

template<
    typename Type = Real
>
class Control : public Procedure {
public:
    Type* const control;
    Type value;

    Control(Type* const control,const Type& value=0):
        control(control),
        value(value){
    }
    
    void reset(void){
    }

    void operate(){
        *control = value;
    }
};


class CONST : public Control<Real> {
public:
    CONST(Real* const control,const Real& value=0):
        Control(control,value){
    }
    
    String signature(void){
        return boost::str(boost::format("CONST %s NA NA NA NA NA NA NA NA NA")
            %value
        );
    }
};

class BIPR : public Control<Real> {
public:

    Real starting;
    
    Real* const index;
    Real* const benchmark;

    typedef Fsl::Math::Series::Filters::Ema Ema;
    Ema ema;
    
    Real multiplier;
    
    RestrictProportionalChange changes;
    RestrictValue values;
    
    BIPR(Real* const control,const Real& starting, Real* const index, Real* const benchmark, const Real& responsiveness=1,const Real& multiplier=1,const Real& change_min=0,const Real& change_max=1,const Real& value_min=0,const Real& value_max=INFINITY):
        Control(control,starting),
        starting(starting),
        index(index),
        benchmark(benchmark),
        ema(responsiveness),
        multiplier(multiplier),
        changes(change_min,change_max),
        values(value_min,value_max)
        {
        reset();
    }
    
    String signature(void){
        return boost::str(boost::format("BIPR %s %s %s %s %s %s %s NA NA NA")
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
        Real last = value;
        Real current = *index;
        Real benchmark_value = *benchmark;
        Real smooth = ema.update(current);
        value = smooth/benchmark_value * multiplier;
        value = changes.restrict(value,last);
        value = values.restrict(value);
        Control::operate();
        return *this;
    }
    
};

} //end namespace Procedures
} //end namespace Management
} //end namespace Fsl
