#pragma once

#include <fsl/common.hpp>

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

//! Double-logistic function
class DoubleLogistic : public Function {
public:

    double inflection_1;
    double inflection_2_delta;
    double steepness_1;
    double steepness_2;

    double value(const double& x) const {
        double a = 1.0/(1.0+std::pow(19,(inflection_1-x)/steepness_1));
        double b = 1.0/(1.0+std::pow(19,(x-(inflection_1+inflection_2_delta))/steepness_2));
        double c = 1.0/(1.0+std::pow(19,
            inflection_1-(
                (inflection_1*steepness_2+(inflection_1+inflection_2_delta)*steepness_1)/
                (steepness_1+steepness_2)
            )/steepness_1
        ));
        return std::min(a,b)/c;
    }

}; // end class DoubleLogistic

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
