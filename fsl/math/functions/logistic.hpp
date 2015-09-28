#pragma once

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

/**
 * Logistic function parameterised with inflection and
 * steepness parameters
 */ 
class Logistic : public Function {
public:
    
    /**
     * Value of x at which y==0.50
     */
    double inflection;

    /**
     * Difference between the value of x where y==0.95 and
     * inflection point
     */
    double steepness;

    double value(const double& x) const {
        return 1.0/(1.0+std::pow(19,(inflection-x)/steepness));
    }

}; // end class Logistic

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
