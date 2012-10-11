#pragma once

//For seeding random number generator with current time
#include <ctime>

//For random number scaffolding...
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
//For forward decalarations of boost::math::mean etc...
#include <boost/math/distributions/uniform.hpp>
//For default random number generation...
#include <boost/random/uniform_01.hpp>

#include <figo/common.hpp>

namespace Figo {
namespace Math {
namespace Probability {
	
/*!
A scaffolding object that holds the random number generator used in the random() methods
*/
struct Generator : boost::mt19937 {
	Generator(void){
		//! Set random number generator seed using current time
		seed(static_cast<unsigned int>(std::time(0)));
	}
} Generator;

/*!
A base implementation class for all probability distributions
	
All probability distributions have a member called data_ which is a boost::math distribution.
(The alternative of deriving from both boost::math distributions caused a mysterious memory bug in testing).
Random variates are produced using boost::random distributions within the random() method.
Specific classes might overide the random() method to provide greate efficiency (?) by using the boost::random distributions directly, rather than using quantile().

Boost::math defines a number of non-member properties that are common to all distributions:
	'cdf','complement','chf','hazard','kurtosis','kurtosis_excess','mean','median','mode','pdf','range','quantile','skewness','standard_deviation','support','variance'
these are wrapped into methods.
*/
template<
	typename Derived
>
class Distribution {
public:
	
	void reset(void){
	}
	
	double mean(void) const {
		return boost::math::mean(static_cast<const Derived&>(*this).boost_dist());
	}
	
	double median(void) const {
		return boost::math::median(static_cast<const Derived&>(*this).boost_dist());
	}

	double mode(void) const {
		return boost::math::mode(static_cast<const Derived&>(*this).boost_dist());
	}
	
	double sd(void) const {
		return boost::math::standard_deviation(static_cast<const Derived&>(*this).boost_dist());
	}
	
	double variance(void) const {
		return boost::math::variance(static_cast<const Derived&>(*this).boost_dist());
	}

	double skewness(void) const {
		return boost::math::skewness(static_cast<const Derived&>(*this).boost_dist());
	}
	
	double kurtosis(void) const {
		return boost::math::kurtosis(static_cast<const Derived&>(*this).boost_dist());
	}

	double kurtosis_excess(void) const {
		return boost::math::kurtosis_excess(static_cast<const Derived&>(*this).boost_dist());
	}
	
	//! @todo implement Duple<> to provide for range and support
	/*
	Duple<> range(void) const {
		return boost::math::range(Derived::dist());
	}

	Duple<> support(void) const {
		return boost::math::support(Derived::dist());
	}
	*/
	
	double pdf(const double& x) const {
		return boost::math::pdf(static_cast<const Derived&>(*this).boost_dist(),x);
	}
	
	double cdf(const double& x) const {
		return boost::math::cdf(static_cast<const Derived&>(*this).boost_dist(),x);
	}
	
	double quantile(const double& p) const {
		return boost::math::quantile(static_cast<const Derived&>(*this).boost_dist(),p);
	}
	
	template<
		typename Function,
		typename... Parameters
	>
	double integrate(const double& from,const double& to,Function function,Parameters... parameters) const {
		/*!
		Calculate the integral of distibution times function (called with parameters).  The first argument of function is the x value of distibutuion.
		Simpson's composite rule
		*/
		int n = 10;
		n = n+(n%2); //This ensures that n is even
		double interval =  (to-from) / n;

		double ans = function(from,parameters...)*pdf(from)+function(to,parameters...)*pdf(to);                                                                       
		double x = from + interval;
		int m = 4;
		for(int i=1;i<n;i++){
			ans += m*function(x,parameters...)*pdf(x);
			m = 6-m;
			x += interval;
		}
		return  interval * ans / 3;
	}
	
	template<
		typename Function,
		typename... Parameters
	>
	double integrate(Function function,Parameters... parameters) const {
		return  integrate(quantile(0.0005),quantile(0.9995),function,parameters...)/0.999;
	}
	
	double random(void) const {
		/*!
		A generalised means of generating a random number for a distribution. A specific distribution might override this for efficiency
		*/
		boost::uniform_01<> dist;
		boost::variate_generator<boost::mt19937&, boost::uniform_01<> > randomVariate(Generator,dist);
		return quantile(randomVariate());
	}
};

}}}
