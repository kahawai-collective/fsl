#pragma once

#include <fisl/common.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
class Threshold : public Function {
public:
    double inflection;
    
    double operator(const double& x){
        return (x<inflection)?0:1;
    }
};

}}}
