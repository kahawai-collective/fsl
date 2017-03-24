#pragma once

#include <stencila/structure.hpp>
#include <stencila/mirror.hpp>
using Stencila::Structure;
using Stencila::Mirrors::Mirror;

#include <fsl/math/probability/fixed.hpp>
using Fsl::Math::Probability::Fixed;

namespace Fsl {
namespace Estimation {


template<
    class Distribution
>
class Variate : public Distribution {
public:

    double value;

    Variate(void):
        Distribution(){
    }

    Variate(const double& value):
        Distribution(value),
        value(value){
    }

    Variate(const Distribution& distribution):
        Distribution(distribution){
    }

    template<typename... Args>
    Variate(const double& value, const Args&... args):
        Distribution(args...),
        value(value) {
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(value, "value");
        Distribution::reflect(mirror);
    }

    operator double (void) const {
        return value;
    }

    void randomize (void) {
        value = Distribution::random();
    }

}; // class Variate

} // namespace Estimation
} // namespace Fsl
