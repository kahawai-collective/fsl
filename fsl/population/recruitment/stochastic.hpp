#pragma once

#include <fsl/population/recruitment/beverton-holt.hpp>
#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/probability/normal.hpp>
#include <fsl/math/series/autocorrelation.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {
	
template<
	class Relationship,
	class Variation,
	class Autocorrelation
>
class Stochastic;
	
template<
	class Relationship
>
class Stochastic<
	Relationship,
	Math::Probability::Lognormal,
	Math::Series::Autocorrelation
> {
public:
	Relationship relationship;
	Math::Probability::Normal variation;
	Math::Series::Autocorrelation autocorrelation;
	
	double spawners;
	
	double recruits(void) {
		double deviation; 
		deviation = variation.random();
		deviation = autocorrelation.value(deviation);
		double multiplier = std::exp(deviation-0.5*variation.sd()*variation.sd());
		return relationship.recruits(spawners)*multiplier;
	}
};

}
}
}