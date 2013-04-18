#pragma once

#include <fsl/population/recruitment/beverton-holt.hpp>
#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/probability/normal.hpp>
#include <fsl/math/series/autocorrelation.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {

template<
    class Relationship = BevertonHolt,
    class Variation = Math::Probability::Lognormal,
    class Autocorrelation = Math::Series::Autocorrelation
>
class Stochastic;

template<
    class Relationship_
>
class Stochastic<
    Relationship_,
    Math::Probability::Lognormal,
    Math::Series::Autocorrelation
> {
public:
    typedef Relationship_ Relationship;
    Relationship relationship;
    
    Math::Probability::Normal variation;
    Math::Series::Autocorrelation autocorrelation;

    double operator()(const double& spawners) {
        double deviation = variation.random();
        deviation = autocorrelation(deviation);
        double multiplier = std::exp(deviation-0.5*variation.sd()*variation.sd());
        return relationship(spawners)*multiplier;
    }
};

}
}
}