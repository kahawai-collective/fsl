#pragma once

namespace Fsl {
namespace Management {
namespace Procedures {

template<class Derived>
class Evaluation {
public:

    int run(void){
        static_cast<Derived*>(this)->priors();
        static_cast<Derived*>(this)->posteriors();
        
        return 0;
    }
    
}; //end class Evaluation

} //end namespace Fsl
} //end namespace Management
} //end namespace Procedures
