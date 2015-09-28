#pragma once

#include <cmath>

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {
    
/**
 * Power function
 */
class Power : public Function<Power> {
public:
    /**
     * Value of function when x==1
     */
    double a;

    /**
     * Exponent of power function
     */
    double b;

    double value(const double& x) const {
        return a*std::pow(x,b);
    }

}; // end class Power

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
