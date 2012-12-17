#pragma once

#include <fisl/common.hpp>
#include <fisl/real.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
//! Logistic function parameterised
class Logistic : public Function {
public:
    //! Value of x at which y==0.50
	Real inflection;
	
	//! Difference between the value of x where y==0.95 and
	//! inflection point
	Real steepness;
    
    Real operator()(const double& x){
        return 1.0/(1.0+std::pow(19,(inflection-x)/steepness));
    }
};

}}}
