#pragma once

#include <fsl/math/probability/normal.hpp>
#include <fsl/math/probability/truncated.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Fsl::Math::Probability::Normal;
using Fsl::Math::Probability::Truncated;

class AutocorrelationBentley2012 : public Truncated<Normal> {
public:
	AutocorrelationBentley2012(void):
		Truncated<Normal>(0.4,0.2,0,1){
	}
};

}
}
}
}
