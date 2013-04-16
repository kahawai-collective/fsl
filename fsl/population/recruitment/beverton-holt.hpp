#pragma once

namespace Fsl {
namespace Population {
namespace Recruitment {

/*!
Beverton-Holt stock recruitment relationship

@todo Prevent steepness being set outside of 0.2-1 range
@todo Prevent r0 and s0 being set -ve
*/
class BevertonHolt {

private:
	double r0_;
	double s0_;
	double steepness_;
	double stock_;
	double recruits_;
	
public:

	double steepness(void) const {
		return steepness_;
	}
	
	BevertonHolt& steepness(const double& steepness) {
		steepness_ = steepness;
		return *this;
	}

	double alpha(void) const {
		return 4*r0_*steepness_/(5*steepness_-1);
	}

	double beta(void) const {
		return -(s0_*steepness_-s0_)/(5*steepness_-1);
	}

	BevertonHolt& operator()(const double& stock) {
		stock_ = stock;
		recruits_ = 4*steepness_*r0_*stock_/((5*steepness_-1)*stock_+s0_*(1-steepness_));
		return *this;
	}
};

}
}
}
