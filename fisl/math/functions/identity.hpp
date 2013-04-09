#pragma once

#include <fisl/common.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
class Identity : public Function {
public:
    
    double operator()(const double& x) const {
        return x;
    }
}; // end class Identity

} // end namespace Fisl
} // end namespace Math
} // end namespace Functions
