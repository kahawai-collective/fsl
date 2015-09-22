#pragma once

#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Population {
namespace Growth {

using Fsl::Math::Functions::Function; 

/*!
von Bertallanfy growth function
*/
class VonBert : public Function {
public:

    double k;
    double linf;
    double t0;

    VonBert(double k, double linf, double t0=0):
    	k(k),
    	linf(linf),
    	t0(t0){
	}

    double operator()(const double& age) const {
        return linf*(1-std::exp(-k*(age-t0)));
    }
};

}
}
}
