#pragma once

#include <boost/math/distributions/beta.hpp>

#include <figo/math/probability/distribution.hpp>

namespace Figo {
namespace Math {
namespace Probability {
	
class Beta : public Distribution<Beta> {
public:
    double alpha;
    double beta;

    using Distribution<Beta>::random;
    
    Beta(const double& alpha_=0, const double& beta_=0):
        alpha(alpha_),
        beta(beta_){        
    }

    Beta& mean_sd(const double& mean, const double& sd){
        double var = sd*sd;
        alpha = boost::math::beta_distribution<>::find_alpha(mean,var);
        beta = boost::math::beta_distribution<>::find_beta(mean,var);
        return *this;
    }

    boost::math::beta_distribution<> boost_dist(void) const {
        return boost::math::beta_distribution<>(alpha,beta);
    }
};

}}}
