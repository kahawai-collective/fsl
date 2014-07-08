#pragma once

#include <boost/math/distributions/uniform.hpp>
#include <boost/random/uniform_real.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Uniform : public Distribution<Uniform> {
public:

    double lower;
    double upper;

    Uniform(double lower = NAN, double upper = NAN):
        lower(lower),
        upper(upper){
    }
    
    bool valid(void) const {
        return std::isfinite(lower) and std::isfinite(upper) and lower<upper;
    }

    boost::math::uniform boost_dist(void) const {
        return boost::math::uniform(lower,upper);
    }

    double minimum(void) const {
        return lower;
    }

    double maximum(void) const {
        return upper;
    }
	
	double mean(void) const {
		return (lower+upper)/2; 
	}

    double sd(void) const {
        return std::sqrt(std::pow(upper-lower,2)/12.0); 
    }
    
    double random(void) const {
        // If lower and upper are equal then boost random number generator
        // will loop endlessly attempting to create a valid value. So escape that condition...
        if(lower==upper) return lower;
        else{
            boost::uniform_real<> distr(lower,upper);
            boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
            return randomVariate();
        }
    }

    bool accept(const double& value) const {
        return value>=lower and value<=upper;
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(lower,"lower")
            .data(upper,"upper")
        ;
    }
};

}}}
