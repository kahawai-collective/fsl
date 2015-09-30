#pragma once

namespace Fsl {
namespace Management {

class Procedure {
public:

    std::string flags;

    std::string signature(void){
        return flags;
    }

    void reset(void){
    }

    void operate(){
    }
};

template<class Procedure=void> 
class ProcedureDynamic;

template<>
class ProcedureDynamic<void> {
public:
    virtual std::string signature(void) = 0;
    virtual void reset(void) = 0;
    virtual void operate(uint time) = 0;
};

template<
    class Procedure
>
class ProcedureDynamic : public ProcedureDynamic<void>, public Procedure {
public:

    template<
        typename... Args
    >
    ProcedureDynamic(Args... args):
        Procedure(args...){
    }

    virtual std::string signature(void){
        return Procedure::signature();
    }
    virtual void reset(void){
        Procedure::reset();
    }
    virtual void operate(uint time){
        Procedure::operate(time);
    }
};

class ProcedureAny {
private:
    ProcedureDynamic<>* instance;
    
public:

    ProcedureAny(ProcedureDynamic<>* instance = 0):
        instance(instance){
    }
    
    ProcedureAny& operator=(ProcedureDynamic<>* instance){
        instance = instance;
        return *this;
    }
    
    std::string signature(void){
        return instance->signature();
    }
    
    void reset(void){
        instance->reset();
    }
    
    void operate(uint time){
        instance->operate(time);
    }

};

template<
    typename Type = double
>
class ControlProcedure : public Procedure {
public:
    Type* const control;
    Type value;

    ControlProcedure(Type* const control,const Type& value = 0):
        control(control),
        value(value){
    }

    void operate(uint time){
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
    
    void reset(void){
        ControlProcedure<Type>::value = starting;
    }
};

} // namespace Management
} // namespace Fsl
