#pragma once

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

class Constant : public Function<Constant> {
private:

	double value_;

public:

	const double& value(void) const {
		return value_;
	}

	double& value(void) {
		return value_;
	}

	Constant& value(const double& value) {
		value_ = value;
		return *this;
	}

    double value(const double& x){
        return value_;
    }

}; // end class Constant

} // end namespace Functions
} // end namespace Math
} // end namespace Fsl
