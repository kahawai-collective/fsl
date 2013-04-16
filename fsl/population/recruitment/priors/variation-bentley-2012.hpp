#pragma once

#include <fsl/math/probability/lognormal.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Fsl::Math::Probability::Lognormal;

/*!

A prior for the standard deviation of recruitment variation

*/
class VariationBentley2012 : public Lognormal {
public:
	VariationBentley2012(void):
		Lognormal(0.6,0.2){
	}
};

}
}
}
}
