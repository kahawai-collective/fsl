/*!
Priors based on analysis of Fishbase
*/
#pragma once

#include <figo/math/probability/lognormal.hpp>
#include <figo/math/statistics/multivariate/regression.hpp>

namespace Figo {
namespace Population {
namespace Growth {
namespace Priors {
	
using Figo::Math::Probability::Lognormal;
using Figo::Math::Statistics::Multivariate::Regression;
	
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