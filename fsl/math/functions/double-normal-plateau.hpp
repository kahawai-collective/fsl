#pragma once

#include <fsl/common.hpp>

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

class DoubleNormalPlateau : public Function {
public:

    double inflection_1;
    double inflection_2_delta;
    double steepness_1;
    double steepness_2;

    double operator()(const double& x) const {
        if(x<=inflection_1) return std::pow(2,-std::pow((x-inflection_1)/steepness_1,2));
        else if(x>inflection_1+inflection_2_delta) return std::pow(2,-std::pow((x-(inflection_1+inflection_2_delta))/steepness_2,2));
        else return 1;
    }

}; // end class DoubleNormalPlateau

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
