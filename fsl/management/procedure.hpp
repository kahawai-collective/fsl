#pragma once

namespace Fsl {
namespace Management {

class Procedure {
public:

    std::string flags;

    virtual std::string signature(void) const {
        return flags;
    }

    virtual void reset(void){
    }

    virtual void operate(uint time){
    }
};

template<
    typename Type = double
>
class ControlProcedure : public Procedure {
public:
    Type* control;
    Type value;

    ControlProcedure(Type* control,const Type& value = 0):
        control(control),
        value(value){
    }

    virtual void operate(uint time){
        *control = value;
    }
};

/**
 * Most management procedures are dynamic
 */
template<
    typename Type = double
>
class DynamicControlProcedure : public ControlProcedure<Type> {
public:
    Type starting;

    DynamicControlProcedure(Type* const control,const Type& starting = 0):
        ControlProcedure<Type>(control,starting),
        starting(starting){
    }
    
    virtual void reset(void){
        ControlProcedure<Type>::value = starting;
    }
};

} // namespace Management
} // namespace Fsl
