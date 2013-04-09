#pragma once

#include <cmath>

#include <fisl/common.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
class Power : public Function {
private:
    double a_;
    double b_;
    
public:

    double a(void) const {
        return a_;
    };
    
    Power& a(const double& value){
        a_ = value;
        return *this;
    }
    
    double b(void) const {
        return b_;
    };
    
    Power& b(const double& value){
        b_ = value;
        return *this;
    }

    double operator()(const double& x){
        return a_*std::pow(x,b_);
    }
}; // end class Power

} // end namespace Fisl
} // end namespace Math
} // end namespace Functions
