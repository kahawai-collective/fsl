/*!
Priors based on analysis of Fishbase
*/
#pragma once

#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/statistics/multivariate/regression.hpp>

namespace Fsl {
namespace Population {
namespace Growth {
namespace Priors {
	
using Fsl::Math::Probability::Lognormal;
using Fsl::Math::Statistics::Multivariate::Regression;
	
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