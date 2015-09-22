#pragma once

#include <boost/random/discrete_distribution.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

template<
    typename Type,
    int Size
>
class Discrete : public Distribution<Uniform> {
public:

    std::array<Type,Size> levels;
    std::array<double,Size> densities;

    Discrete(void){
    }

    Discrete(std::array<Type,Size> levels, std::array<double,Size> densities):
        levels(levels),
        densities(densities),
        rand_distr_(densities.begin(),densities.end()){
    }
    
    Type random(void) const {
        boost::variate_generator<boost::mt19937&,decltype(rand_distr_)> randomVariate(Generator,rand_distr_);
        auto which = randomVariate();
        return levels[which];
    }

private:

    boost::random::discrete_distribution<> rand_distr_;
};

}}}
