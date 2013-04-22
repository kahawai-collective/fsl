#pragma once

#include <iostream>
#include <typeinfo>

namespace Fsl {
namespace Management {
namespace Procedures {

class PriorDocumenter{
public:/*
    template<
        class Target,
        class Distribution
    >
    PriorDocumenter& operator()(Target& target, std::string name, double (Target::* getter)(void) const, Target& (Target::* setter)(const double& value),const Distribution& distibution){
        std::cout<<typeid(target).name()<<" "<<typeid(setter).name()<<" "<<typeid(distibution).name()<<std::endl;
        return *this;
    }
    */
    template<
        class Property,
        class Distribution
    >
    PriorDocumenter& operator()(const Property& property,const Distribution& distibution){
        std::cout<<property.name()<<" ~ "<<typeid(distibution).name()<<std::endl;
        return *this;
    }
    
    template<
        class Property,
        class Distribution
    >
    PriorDocumenter& operator()(const char* name, const Property& property,const Distribution& distibution){
        std::cout<<name<<" ~ "<<typeid(distibution).name()<<std::endl;
        return *this;
    }
};

class LikelihoodDocumenter{
public:
    template<
        class Target,
        class Distribution
    >
    LikelihoodDocumenter& operator()(Target& target, std::string name, double (Target::* getter)(void) const, Target& (Target::* setter)(const double& value),const Distribution& distibution){
        std::cout<<typeid(target).name()<<" "<<typeid(setter).name()<<" "<<typeid(distibution).name()<<std::endl;
        return *this;
    }
    
};


template<
    class Derived
>
class Evaluation {
public:

    struct Conditioning {
        int start;
        int finish;
    };
    Conditioning conditioning;

    void prior(void){
        
    }

    void document(void){
        PriorDocumenter priors;
        static_cast<Derived*>(this)->priors(priors);
        
        LikelihoodDocumenter likelihoods;
        for(int time=conditioning.start;time<=conditioning.finish;time++){
            static_cast<Derived*>(this)->likelihood(likelihoods,time);
        }
    }

    void condition(void){
        
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
