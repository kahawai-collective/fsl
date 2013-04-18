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

    FSL_PROPERTY(VonBert,k,double)
    FSL_PROPERTY(VonBert,linf,double)
    FSL_PROPERTY(VonBert,t0,double)

public:

    double operator()(const double& age) const {
        return linf_*(1-std::exp(-k_*(age-t0_)));
    }
};

}
}
}
