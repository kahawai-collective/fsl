#ifdef FISL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/math/functions/function.hpp>
#include <fsl/math/functions/identity.hpp>

BOOST_AUTO_TEST_SUITE(cached)

using namespace Fsl::Math::Functions;
Cached<Identity,10> c1;

BOOST_AUTO_TEST_CASE(reset_plain){
    c1.reset();
    BOOST_CHECK_EQUAL(c1(3),3);
    BOOST_CHECK_EQUAL(c1.sum(),0+1+2+3+4+5+6+7+8+9);
}

BOOST_AUTO_TEST_CASE(reset_base_step){
    c1.reset(0.123,4.56);
    BOOST_CHECK_EQUAL(c1(3),0.123+4.56*3);
}

BOOST_AUTO_TEST_CASE(reset_function){
    c1.reset([](int index)->int{
        int values[10]={
            10,9,8,7,6,5,4,3,2,1
        };
        return values[index];
    });
    BOOST_CHECK_EQUAL(c1(0),10);
    BOOST_CHECK_EQUAL(c1(3),7);
    BOOST_CHECK_EQUAL(c1(9),1);
}

BOOST_AUTO_TEST_SUITE_END()
