#pragma once

#include <fisl/math/probability/lognormal.hpp>
#include <fisl/math/statistics/multivariate/regression.hpp>

namespace Fisl {
namespace Population {
namespace Mortality {
namespace Priors {
namespace Bentley2010 {
	
using Fisl::Math::Statistics::Multivariate::Regression;

class MFromK : public Regression {
/*
Priors based on analysis of Fishbase data by Nokome Bentley (2010)
*/
public:
	MFromK(void):
		Regression(0.46,0.89,0.44){}
		
	double random(const double& k) const {
		return std::exp(Regression::random(std::log(k)));
	}
};

}
}
}
}
}