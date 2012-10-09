#pragma once

#include <figo/common.hpp>
#include <figo/math/functions/function.hpp>

namespace Figo {
namespace Math {
namespace Functions {
    
class Identity : public Function {
public:
    
    double operator()(const double& x) const {
        return x;
    }
};

}}}
