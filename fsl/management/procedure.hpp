#pragma once

#include <iostream>
#include <typeinfo>

#include <fsl/date.hpp>

namespace Fsl {
namespace Management {

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

} //end namespace Management
} //end namespace Fsl
