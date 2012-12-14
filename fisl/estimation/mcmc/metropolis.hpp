#pragma once

#include <fstream>

#include <stencila/datatable.hpp>
#include <stencila/dataset.hpp>
#include <stencila/hashing.cpp>

#include <fisl/real.hpp>
#include <fisl/array.hpp>
#include <fisl/math/probability/uniform.hpp>
#include <fisl/math/probability/normal.hpp>

namespace Fisl {
namespace Estimation {
namespace Mcmc {
	
using Stencila::Dataset;
using Stencila::Datatable;

/*!
@brief Metropolis Markov chain Monte Carlo algorithm

The Metropolis algorithm is probably the simplest Markov chain Monte Carlo (MCMC)
algorithm. It is a special, simplified case of the Hastings-Metropolis algorithm in
which the proposal jump distribution is symetric and therefore the acceptance
ratio for a jump is simply the ratio of the likelihoods.

This implementation uses the gaussian distribution

@author Nokome Bentley
*/
template<
	class Derived,
	int Parameters
>
class Metropolis : public Dataset {
public:
	//! Vector that represents the paramter values at the end of the chain
	Array<Real,Parameters> values;
	Real ll;
	
	//! Variances for proposals. These need to be tuned
	//! by user to obtain an acceptance ratio (20-70%)
	Array<Real,Parameters> variances;
	
	unsigned int iterations;
	unsigned int accepted;
	Real acceptance;
	
	Datatable samples;
	
	Metropolis(void){
		using Stencila::Integer;
		using Stencila::Real;
		samples = create("samples",
			"iteration",Real,
			"acceptance",Real,
			"log_like",Real
		);
		//! @todo add all parameters
		for(int par=0;par<Parameters;par++) samples.add("p"+boost::lexical_cast<std::string>(par),Real);
	}
	
	void reset(void){
		iterations = 0;
		accepted = 0;
		acceptance = 0;
		ll = -INFINITY;
	}
	
	void step(void){
		typedef Math::Probability::Normal Normal;
		typedef Math::Probability::Uniform Uniform;
		
		//! Generate a proposal for each parameter
		Array<Real,Parameters> proposal;
		for(int par=0;par<Parameters;par++){
			Real value = values(par);
			Real variance = variances(par);
			proposal(par) = Normal(value,std::sqrt(variance)).random();
		}
		//! Obtain likelihood for proposal
		Real ll_proposal = static_cast<Derived*>(this)->log_like(proposal);
		//! Test to see if this proposal will be accepted
		Real ratio = ll_proposal-ll;
		iterations++;
		if(ratio>std::log(Uniform().random())){
			values = proposal;
			ll = ll_proposal;
			
			iterations++;
			acceptance = accepted/float(iterations);
			
			//Insert the current values into the samples datatable
			std::vector<double> row(3+Parameters);
			row[0] = iterations;
			row[1] = acceptance;
			row[2] = ll;
			for(int par=0;par<Parameters;par++) row[3+par] = values(par);
			samples.append(row);
		}
	}
	
	void run(unsigned int n){
		for(unsigned int i=0;i<n;i++){
			step();
		}
	}
};

}
}
}