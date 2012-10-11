#pragma once

#include <boost/math/distributions/uniform.hpp>
#include <boost/random/uniform_real.hpp>

#include <figo/math/probability/distribution.hpp>

namespace Figo {
namespace Math {
namespace Probability {

using namespace Figo;

class Uniform : public Distribution<Uniform> {
public:
    double lower;
    double upper;
    

    Uniform(double lower_=0, double upper_=1){
        lower = lower_;
        upper = upper_;
    }
    
    boost::math::uniform boost_dist(void) const {
        return boost::math::uniform(lower,upper);
    }
    
    double random(void) const {
        boost::uniform_real<> distr(lower,upper);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }

    bool accept(const double& value) const {
        return value>=lower and value<=upper;
    }
};

}}}
