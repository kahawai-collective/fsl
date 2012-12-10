#pragma once

#include <fisl/math/functions/function.hpp>

namespace Fisl {
namespace Population {
namespace Growth {
	
using Fisl::Math::Functions::Function; 

/*!
von Bertallanfy growth function
*/
class VonBert : public Function {
public:
	double k;
	double linf;
	double t0;
	
	double operator()(const double& age) const {
		return linf*(1-std::exp(-k*(age-t0)));
	}
};

}
}
}
