#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fisl/population/structure/aged.hpp>
#include <fisl/population/structure/gendered.hpp>

BOOST_AUTO_TEST_SUITE(population_structure_aged)

using namespace Fisl::Population::Structure;

BOOST_AUTO_TEST_CASE(simple){
    /*
    Gendered<Aged<double,10>> pop = {10,9,8,7,6,5,4,3,2,1};
    pop.ageing(20);
    BOOST_CHECK_EQUAL(pop(0),20);
    BOOST_CHECK_EQUAL(pop(1),10);
    BOOST_CHECK_EQUAL(pop(9),3);
    */
}

BOOST_AUTO_TEST_SUITE_END()
