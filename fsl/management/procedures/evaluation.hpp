#pragma once

#include <iostream>
#include <typeinfo>

namespace Fsl {
namespace Management {
namespace Procedures {

class PriorDocumenter{
public:
    template<
        class Target,
        class Prior
    >
    PriorDocumenter& operator()(Target& target, std::string name, double (Target::* getter)(void) const, Target& (Target::* setter)(const double& value),const Prior& prior){
        std::cout<<typeid(target).name()<<" "<<typeid(setter).name()<<" "<<typeid(prior).name()<<std::endl;
        return *this;
    }
    
};


template<class Derived>
class Evaluation {
public:

    void prior(void){
        
    }

    void document(void){
        PriorDocumenter priors;
        static_cast<Derived*>(this)->priors(priors);
    }

    void condition(void){
        //static_cast<Derived*>(this)->log_likelihood();
    }

    void run(void){
        document();
        condition();
        //evaluate();
    }
    
}; //end class Evaluation

} //end namespace Procedures
} //end namespace Management
} //end namespace Fsl
