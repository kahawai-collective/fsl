#pragma once

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
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
        return (x<inflection_)?0:1;
    }
}; // end class Threshold

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
