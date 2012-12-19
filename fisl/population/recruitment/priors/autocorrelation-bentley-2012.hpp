#pragma once

#include <fisl/math/probability/normal.hpp>
#include <fisl/math/probability/truncated.hpp>

namespace Fisl {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Fisl::Math::Probability::Normal;
using Fisl::Math::Probability::Truncated;

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
