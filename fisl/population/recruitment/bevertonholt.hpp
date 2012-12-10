#pragma once

#include <fisl/real.hpp>

namespace Fisl {
namespace Population {
namespace Recruitment {

/*!
Beverton-Holt stock recruitment relationship

@todo Prevent steepness being set outside of 0.2-1 range
@todo Prevent r0 and s0 being set -ve
*/
class BevertonHolt {
public:
	Real r0;
	Real s0;
	Real steepness;
	Real stock;
	Real recruits;

	Real alpha(void) const {
		return 4*r0*steepness/(5*steepness-1);
	}

	Real beta(void) const {
		return -(s0*steepness-s0)/(5*steepness-1);
	}

	BevertonHolt& operator()(const double& stock_) {
		stock = stock_;
		recruits = 4*steepness*r0*stock/((5*steepness-1)*stock+s0*(1-steepness));
		return *this;
	}
};

}
}
}
