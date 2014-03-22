#pragma once

#include <fsl/math/probability/normal.hpp>
#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/series/autocorrelation.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {

template<
    class Variation = Math::Probability::Lognormal
>
class Autocorrelated;

template<>
class Autocorrelated<
    Math::Probability::Lognormal
> {
private:
    Math::Series::Autocorrelation autocorrelation;

public:
    double sd;
    double autocor;

    double random(void) {
        Normal normal(0,sd);
        double deviation = normal.random();
        deviation = autocorrelation(deviation);
        double multiplier = std::exp(deviation-0.5*sd*sd);
        return multiplier;
    }
};

}
}
}