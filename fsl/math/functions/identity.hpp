#pragma once

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {
    
class Identity : public Function {
public:
    
    double operator()(const double& x) const {
        return x;
    }
}; // end class Identity

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
