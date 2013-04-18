#pragma once

#include <fsl/common.hpp>

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

//! Logistic function parameterised
class Logistic : public Function {

    //! @property inflection
    //! Value of x at which y==0.50
    FSL_PROPERTY(Logistic,inflection,double)

    //! @property steepness
    //! Difference between the value of x where y==0.95 and
    //! inflection point
    FSL_PROPERTY(Logistic,steepness,double)

public:

    double operator()(const double& x){
        return 1.0/(1.0+std::pow(19,(inflection_-x)/steepness_));
    }

}; // end class Logistic

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
