#ifdef FSL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/population/models/model.hpp>

BOOST_AUTO_TEST_SUITE(population_models_aged_gendered)

using namespace Fsl::Population::Models;

BOOST_AUTO_TEST_CASE(simple){
    Aged<
        10//Ages
    > pop;

    pop.sizes.k(0.7)
             .linf(100)
             .t0(0);
    pop.sizes.reset();

    BOOST_CHECK_EQUAL(pop.sizes(0),0);

    pop.maturities.inflection(4);
    pop.maturities.reset();

    BOOST_CHECK_EQUAL(pop.maturities(3),0);
    BOOST_CHECK_EQUAL(pop.maturities(5),1);
}

BOOST_AUTO_TEST_CASE(simple_2){
    Sexed<
        Aged<
            10//Ages
        >
    > pop;

    pop.males.sizes.k(0.3);
    pop.males.sizes.linf(100);
}

BOOST_AUTO_TEST_SUITE_END()
