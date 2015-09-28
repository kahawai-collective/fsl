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
    
    double value(const double& x){
        return (x<inflection_)?0:1;
    }
}; // class Threshold

} // namespace Functions
} // namespace Math
} // namespace Fsl
