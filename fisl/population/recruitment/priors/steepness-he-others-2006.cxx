#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fstream>

#include <boost/foreach.hpp>

#include <fisl/population/recruitment/priors/steepness_he_2006.hpp>

using Fisl::Population::Recruitment::Priors::SteepnessHe2006;

BOOST_AUTO_TEST_CASE(examples){
	//! Generate examples of priors for different levels of M and sigma
	SteepnessHe2006 prior;
	
	std::ofstream out("steepness_he_2006_tests_relative.tsv");
	out<<"mortality\tsigma\tsteepness\tdensity\n";
	
	double mortalities[] = {0.1,0.3,0.5};
	double sigmas[] = {0.3,0.6,1.2};
	BOOST_FOREACH(double mortality, mortalities){
		BOOST_FOREACH(double sigma, sigmas){
			auto densities = prior.densities(mortality,sigma);
			typedef std::pair<double,double> Density;
			BOOST_FOREACH(Density& density, densities){
				out<<mortality<<"\t"<<sigma<<"\t"<<density.first<<"\t"<<density.second<<"\n";
			}
		}
	}	
}
