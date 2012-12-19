#pragma once

#include <fisl/common.hpp>

#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
//! Logistic function parameterised
class Logistic : public Function {
private:

	double inflection_;
	double steepness_;
	
public:
    //! Value of x at which y==0.50
	double inflection(void) const {
		return inflection_;
	};
	
	Logistic& inflection(const double& value) {
		inflection_ = value;
		return *this;
	};
	
	//! Difference between the value of x where y==0.95 and
	//! inflection point
	double steepness(void) const {
		return steepness_;
	};
	
	Logistic& steepness(const double& value) {
		steepness_ = value;
		return *this;
	};
    
    double operator()(const double& x){
        return 1.0/(1.0+std::pow(19,(inflection_-x)/steepness_));
    }
};

}}}
