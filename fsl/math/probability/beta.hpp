#pragma once

#include <boost/math/distributions/beta.hpp>

#include <fsl/math/probability/distribution.hpp>

namespace Fsl {
namespace Math {
namespace Probability {
	
class Beta : public Distribution<Beta> {
private:
    double alpha_;
    double beta_;

public:

    using Distribution<Beta>::random;
    
    Beta(const double& alpha = NAN, const double& beta = NAN):
        alpha_(alpha),
        beta_(beta){        
    }

    Beta& mean_sd(const double& mean, const double& sd){
        double var = sd*sd;
        alpha_ = boost::math::beta_distribution<>::find_alpha(mean,var);
        beta_ = boost::math::beta_distribution<>::find_beta(mean,var);
        return *this;
    }

    boost::math::beta_distribution<> boost_dist(void) const {
        return boost::math::beta_distribution<>(alpha_,beta_);
    }
};

}}}
