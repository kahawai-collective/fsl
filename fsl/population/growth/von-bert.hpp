#pragma once

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Population {
namespace Growth {
	
using Fsl::Math::Functions::Function; 

/*!
von Bertallanfy growth function
*/
class VonBert : public Function {
private:
	double k_;
	double linf_;
	double t0_;
	
public:

	double k(void) const {
		return k_;
	};
	
	VonBert& k(const double& value){
		k_ = value;
		return *this;
	}
	
	double linf(void) const {
		return linf_;
	};
	
	VonBert& linf(const double& value){
		linf_ = value;
		return *this;
	}
	
	double t0(void) const {
		return t0_;
	};
	
	VonBert& t0(const double& value){
		t0_ = value;
		return *this;
	}

	double operator()(const double& age) const {
		return linf_*(1-std::exp(-k_*(age-t0_)));
	}
};

}
}
}
