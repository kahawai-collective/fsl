#ifdef FSL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/population/recruitment/beverton-holt.hpp>

BOOST_AUTO_TEST_SUITE(bevertonholt)

using namespace Fsl::Population::Recruitment;

BOOST_AUTO_TEST_CASE(simple){

    BevertonHolt bh;
    bh.s0(327070)
      .r0(580823750)
      .steepness(0.746157);

    //Check that at virgin stock size get virgin recruitment
    BOOST_CHECK_CLOSE(bh(bh.s0()),bh.r0(),0.0001);

    //Check that at 20% of virgin get steepness fraction of r0
    BOOST_CHECK_CLOSE(bh(bh.s0()*0.2),bh.r0()*bh.steepness(),0.0001);

}

BOOST_AUTO_TEST_SUITE_END()
