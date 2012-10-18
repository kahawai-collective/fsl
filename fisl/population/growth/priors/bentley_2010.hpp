/*!
Priors based on analysis of Fishbase
*/
#pragma once

#include <fisl/math/probability/lognormal.hpp>
#include <fisl/math/statistics/multivariate/regression.hpp>

namespace Fisl {
namespace Population {
namespace Growth {
namespace Priors {
	
using Fisl::Math::Probability::Lognormal;
using Fisl::Math::Statistics::Multivariate::Regression;
	
class Linf : public Lognormal {
public:
	Linf(void):
		Lognormal(59.3,80.0){}
};

class KFromLinf : public Regression {
public:
	KFromLinf(void):
		Regression(1.47,-0.69,0.64){}
		
	double random(const double& linf) const {
		return std::exp(Regression::random(std::log(linf)));
	}
};

}
}
}
}