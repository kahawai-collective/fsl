#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fisl/population/structure/aged.hpp>
#include <fisl/population/structure/gendered.hpp>
#include <fisl/population/structure/numbers.hpp>

BOOST_AUTO_TEST_SUITE(population_structure_aged)

using namespace Fisl::Population::Structure;

BOOST_AUTO_TEST_CASE(simple){
    Gendered<Aged<Numbers,10>> pop;
	pop(0) = {10,9,8,7,6,5,4,3,2,1};
	pop(1) = {10,9,8,7,6,5,4,3,2,1};
    pop.recruits = 20;
	pop.proportion = 0.5;
	pop.ageing();
    BOOST_CHECK_EQUAL(pop(0)(0),10);
    BOOST_CHECK_EQUAL(pop(1)(0),10);
	BOOST_CHECK_EQUAL(pop(1)(1),10);
}

BOOST_AUTO_TEST_SUITE_END()
