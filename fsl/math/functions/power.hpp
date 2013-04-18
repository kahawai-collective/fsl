#pragma once

#include <cmath>

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {
    
class Power : public Function {

    FSL_PROPERTY(Power,a,double)
    FSL_PROPERTY(Power,b,double)

public:

    double operator()(const double& x){
        return a_*std::pow(x,b_);
    }

}; // end class Power

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
