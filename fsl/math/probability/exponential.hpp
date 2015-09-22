#pragma once

#include <boost/math/distributions/exponential.hpp>
#include <boost/random/exponential_distribution.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {

using namespace Fsl;

class Exponential : public Distribution<Exponential> {
public:

    Exponential(const double& lambda=NAN):
        lambda_(lambda){
    }
    
    boost::math::exponential boost_dist(void) const {
        return boost::math::exponential(lambda_);
    }
    
    double random(void) const {
        boost::exponential_distribution<> distr(lambda_);
        boost::variate_generator<boost::mt19937&,decltype(distr)> randomVariate(Generator,distr);
        return randomVariate();
    }
private:
    double lambda_;
};

}}}
