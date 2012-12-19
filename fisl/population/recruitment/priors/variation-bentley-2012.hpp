#pragma once

#include <fisl/math/probability/lognormal.hpp>

namespace Fisl {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Fisl::Math::Probability::Lognormal;

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
