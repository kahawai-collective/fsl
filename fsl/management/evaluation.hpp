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

    Derived& initialise(void){
        Derived& self = static_cast<Derived&>(*this);
        self.evaluation.initialise();
        return self;
    }

    Derived& document(void){
        Derived& self = static_cast<Derived&>(*this);
        self.conditioning.document(self.model);
        self.evaluation.document(self.model);
        return self;
    }

    Derived& condition(void){
        Derived& self = static_cast<Derived&>(*this);
        self.conditioning.condition(self.model);
        return self;
    }
    
    Derived& evaluate(void){
        Derived& self = static_cast<Derived&>(*this);
        self.evaluation.evaluate(self.model);
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
