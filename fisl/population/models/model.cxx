#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fisl/population/models/model.hpp>

BOOST_AUTO_TEST_SUITE(population_models_aged_gendered)

using namespace Fisl::Population::Models;

BOOST_AUTO_TEST_CASE(simple){
	Aged<
		10//Ages
	> pop;
	
	pop.sizes.k = 0.7;
	pop.sizes.linf = 100;
	pop.sizes.t0 = 0;
	pop.sizes.reset();
	
	BOOST_CHECK_EQUAL(pop.sizes(0),0);
	
	pop.maturities.inflection = 4;
	pop.maturities.reset();
	
	BOOST_CHECK_EQUAL(pop.maturities(3),0);
	BOOST_CHECK_EQUAL(pop.maturities(5),1);
}

BOOST_AUTO_TEST_CASE(simple_2){
	Gendered<
		Aged<
			10//Ages
		>
	> pop;
	
	pop.male.sizes.k = 0.3;
	pop.male.sizes.linf = 100;
}

BOOST_AUTO_TEST_SUITE_END()
