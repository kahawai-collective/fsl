#pragma once

#include <fsl/population/recruitment/beverton-holt.hpp>
#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/probability/normal.hpp>
#include <fsl/math/series/autocorrelation.hpp>

namespace Fsl {
namespace Population {
namespace Recruitment {

template<
    class Variation = Math::Probability::Lognormal,
    class Autocorrelation = Math::Series::Autocorrelation
>
class Stochastic;

template<
    class Relationship
>
class Stochastic<
    Relationship,
    Math::Probability::Lognormal,
    Math::Series::Autocorrelation
> {
public:
    Relationship relationship;
    int seed;
    double sd;
    Math::Series::Autocorrelation autocorrelation;
    
    boost::mt19937 generator;
    
    void initialise(void){
        generator.seed(seed);
    }
    
    double stock_virgin(void) const {
        return relationship.stock_virgin();
    }
    
    double recruits_virgin(void) const {
        return relationship.recruits_virgin();
    }

    double operator()(const double& spawners) {
        boost::normal_distribution<> distr(0,1);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(generator,distr);
        double deviation = randomVariate()*sd;
        
        deviation = autocorrelation(deviation);
        double multiplier = std::exp(deviation-0.5*sd*sd);
        return relationship(spawners)*multiplier;
    }
};

}
}
}