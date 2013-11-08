#pragma once

#include <cmath>

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

class Line : public Function {
public:

    double a;
    double b;

    double operator()(const double& x){
        return a+x*b;
    }

}; // end class Line

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
