#pragma once

#include <iostream>
#include <typeinfo>

#include <fsl/date.hpp>

namespace Fsl {
namespace Management {

template<
    class Derived
>
class Evaluation {
public:

    Derived& document(void){
        Derived& self = static_cast<Derived&>(*this);
        self.conditioning.document(self.model);
        self.evaluation.document(self.model);
        return self;
    }

    Derived& condition(void){
        Derived& self = static_cast<Derived&>(*this);
        self.conditioner.condition(self);
        return self;
    }
    
    Derived& evaluate(void){
        Derived& self = static_cast<Derived&>(*this);
        for(int replicate=0;replicate<self.conditioning.replicates();replicate++){
            self.conditioning.apply(replicate,self.model);
            for(int candidate=0;candidate<self.evaluation.candidates();candidate++){
                self.evaluation.evaluate(candidate,self.model);
            }
        }
        return self;
    }

    void run(void){
        document();
        condition();
        evaluate();
    }
    
}; //end class Evaluation

} //end namespace Management
} //end namespace Fsl
