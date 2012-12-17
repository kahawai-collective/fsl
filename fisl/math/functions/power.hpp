#pragma once

#include <cmath>

#include <fisl/common.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
class Power : public Function {
public:
    double a;
	double b;
    
    double operator()(const double& x){
        return a*std::pow(x,b);
    }
};

}}}
