#pragma once

#include <figo/common.hpp>
#include <figo/math/functions/function.hpp>

namespace Figo {
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
