#pragma once

namespace Fsl {
namespace Management {

class Procedure {
public:

    void signature(void){
        std::runtime_error("Procedure must define a signature");
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
    virtual void operate(void) = 0;
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
    virtual void operate(void){
        Procedure::operate();
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
    
    void operate(void){
        instance->operate();
    }

};

template<
    typename Type = double
>
class ControlProcedure : public Procedure {
public:
    Type* const control;
    Type value;

    ControlProcedure(Type* const control,const Type& value=0):
        control(control),
        value(value){
    }
    
    void reset(void){
    }

    void operate(){
        *control = value;
    }
};

} // namespace Management
} // namespace Fsl
