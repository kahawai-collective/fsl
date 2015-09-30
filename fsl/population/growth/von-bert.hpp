#pragma once

#include <stencila/structure.hpp>
using Stencila::Structure;


namespace Fsl {
namespace Population {
namespace Growth {

/*!
von Bertallanfy growth function
*/
class VonBert : public Structure<VonBert> {
public:

    double k;
    double linf;
    double t0;

    VonBert(){
    }

    VonBert(double k, double linf, double t0=0):
    	k(k),
    	linf(linf),
    	t0(t0){
	}

    double value(const double& age) const {
        return linf*(1-std::exp(-k*(age-t0)));
    }

    template<class Mirror>
    void reflect(Mirror& mirror){
        mirror
            .data(k,"k")
            .data(linf,"linf")
            .data(t0,"t0")
        ;
    }
};

}
}
}
