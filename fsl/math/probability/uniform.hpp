#pragma once

#include <boost/math/distributions/uniform.hpp>
#include <boost/random/uniform_real.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Uniform : public Distribution<Uniform> {
private:
    double lower_;
    double upper_;

public:
    Uniform(double lower = 0, double upper = 1):
        lower_(lower),
        upper_(upper){
    }
    
    boost::math::uniform boost_dist(void) const {
        return boost::math::uniform(lower_,upper_);
    }
    
    double random(void) const {
        // If lower and upper are equal then boost random number generator
        // will loop endlessly attempting to create a valid value. So escape that condition...
        if(lower_==upper_) return lower_;
        else{
            boost::uniform_real<> distr(lower_,upper_);
            boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
            return randomVariate();
        }
    }

    bool accept(const double& value) const {
        return value>=lower_ and value<=upper_;
    }
};

}}}
