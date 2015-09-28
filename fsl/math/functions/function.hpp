#pragma once

#include <fsl/common.hpp>

namespace Fsl {
namespace Math {
namespace Functions {
    
/**
 * A base class for all functions
 * 
 * This class is not expected to be used but it defines the interface
 * that all Functions are expected to have. Functions act as functors with
 * a "call operator" which takes a double and returns a double.
 */
class Function {
public:
    
    /**
     * Get the value of the function
     * @param  x Point at which function is evaluated
     * @return   Value of function
     */
    double value(const double& x){
        return 0;
    }
};

} // end namespace Functions
} // end namespace Math
} // end namespace Fsl
