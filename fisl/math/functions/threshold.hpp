#pragma once

#include <fisl/common.hpp>
#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Math {
namespace Functions {
    
class Threshold : public Function {
private:

	double inflection_;

public:

	double inflection(void) const {
		return inflection_;
	};
	
	Threshold& inflection(const double& value) {
		inflection_ = value;
		return *this;
	};
    
    double operator()(const double& x){
        return (x<inflection)?0:1;
    }
}; // end class Threshold

} // end namespace Fisl
} // end namespace Math
} // end namespace Functions
