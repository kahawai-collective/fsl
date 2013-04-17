#ifdef FSL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/math/probability/beta.hpp>
#include <fsl/math/probability/fixed.hpp>
#include <fsl/math/probability/lognormal.hpp>
#include <fsl/math/probability/normal.hpp>
#include <fsl/math/probability/uniform.hpp>

BOOST_AUTO_TEST_SUITE(cached)

using namespace Fsl::Math::Probability;    

BOOST_AUTO_TEST_CASE(beta){     
    Beta beta;
    beta.mean_sd(0.5,0.1);
    BOOST_CHECK_EQUAL(beta.mean(),0.5);
    BOOST_CHECK_EQUAL(beta.sd(),0.1);
}

BOOST_AUTO_TEST_CASE(fixed){     
    Fixed fixed(3.14);
    BOOST_CHECK_EQUAL(fixed.mean(),3.14);
    BOOST_CHECK_EQUAL(fixed.sd(),0);
}

BOOST_AUTO_TEST_CASE(lognormal){     
    Lognormal lognormal(1,0.5);
    BOOST_CHECK_EQUAL(lognormal.mean(),1);
    BOOST_CHECK_EQUAL(lognormal.sd(),0.5);
}

BOOST_AUTO_TEST_CASE(normal){     
    Normal normal(1,0.5);
    BOOST_CHECK_EQUAL(normal.mean(),1);
    BOOST_CHECK_EQUAL(normal.sd(),0.5);
}

BOOST_AUTO_TEST_CASE(uniform){     
    Uniform uniform(1,2);
    BOOST_CHECK_EQUAL(uniform.mean(),1.5);
}

BOOST_AUTO_TEST_SUITE_END()
