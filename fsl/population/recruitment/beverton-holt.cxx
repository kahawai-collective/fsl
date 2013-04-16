#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/population/recruitment/bevertonholt.hpp>

BOOST_AUTO_TEST_SUITE(bevertonholt)

using namespace Fsl::Population::Recruitment;

BOOST_AUTO_TEST_CASE(simple){

	BevertonHolt bh;
	bh.s0 = 327070;
	bh.r0 = 580823750;
	bh.steepness = 0.746157;
	
	//Check that at virgin stock size get virgin recruitment
	bh(bh.s0);
	BOOST_CHECK_CLOSE((double)bh.recruits(),(double)bh.r0,0.0001);
	
	//Check that at 20% of virgin get steepness fraction of r0
	bh(bh.s0*0.2);
	BOOST_CHECK_CLOSE((double)bh.recruits(),(double)bh.r0*bh.steepness,0.0001);
}

BOOST_AUTO_TEST_SUITE_END()
