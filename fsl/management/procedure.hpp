#pragma once

#include <iostream>
#include <typeinfo>

#include <fsl/date.hpp>

namespace Fsl {
namespace Management {
    
class Procedure {
private:
    
    
    
public:
    template<
        class Monitoring
    >
    Procedure& update(const Date& date,const Monitoring& monitoring){
        return *this;
    }
};

class ProcedureAny {
    
private:
    Procedure* instance_;
    
public:

    ProcedureAny(Procedure* instance = 0):
        instance_(instance){
    }
    
    ProcedureAny& operator=(Procedure& instance){
        instance_ = &instance;
        return *this;
    }

    template<
        class Monitoring
    >
    ProcedureAny& update(const Date& date,const Monitoring& monitoring){
        instance_->update(date,monitoring);
        return *this;
    }
};

} //end namespace Management
} //end namespace Fsl
