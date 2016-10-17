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
template<
	class Derived
>
class Function : public Structure<Derived> {
public:
    
    /**
     * Get the value of the function
     * @param  x Point at which function is evaluated
     * @return   Value of function
     */
    double value(const double& x){
        return 0;
    }

    /**
     * Overloading of `()` operator.
     * 
     * This is used for some functions that can accept either
     * a normal function or a `Function` instance. e.g. `Distribution::integrate()`
     */
    double operator()(const double& x){
        return static_cast<Derived&>(*this).value(x);
    }
};

} // end namespace Functions
} // end namespace Math
} // end namespace Fsl
