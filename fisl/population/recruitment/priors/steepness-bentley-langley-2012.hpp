#pragma once

#include <fisl/math/probability/beta.hpp>

namespace Fisl {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Fisl::Math::Probability::Beta;

/*!

A prior for stock recruitment steepness which uses a transformation of the beta
distribution to limit the range of values to 0.2-1. Parameter values from Table 1 of 
	Bentley & Langley (2012) Feasible stock trajectories: a flexible and efficient
	sequential estimator for use in fisheries management procedures
which notes
	"Based on Myers et al. (1999) median = 0.67, 25th% = 0.48, 75th% = 0.82"

*/
class SteepnessBentleyLangley2012 : public Beta {
public:

	SteepnessBentleyLangley2012(void){
		alpha(1.91),
		beta(1.28){
	}
	
	Real random(void) const {
		return (Beta::random()+0.25)/1.25;
	}
};

}
}
}
