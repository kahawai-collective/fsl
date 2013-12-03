#ifdef FSL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/estimation/mcmc/metropolis.hpp>

namespace Fsl {
namespace Estimation {
namespace Mcmc {
    
BOOST_AUTO_TEST_SUITE(metropolis)

using Fsl::Math::Probability::Normal;
using Fsl::Estimation::Mcmc::Metropolis;

class McmcNormal : public Metropolis<McmcNormal,1> {
public:
    Normal dist;
    
    void reset(void){
        dist = Normal(3.14,0.56);
        values = {1};
        variances = {1}; 
        Metropolis<McmcNormal,1>::reset();
    }
    
    double log_like(const Array<>& parameters){
        double x = parameters(0);
        return std::log(dist.pdf(x));
    }
}; 

BOOST_AUTO_TEST_CASE(test_1){
    McmcNormal m;
    m.reset();
    m.run(10000);
    BOOST_CHECK_CLOSE(m.samples.value<double>("mean(p0)"),3.14,1);
    BOOST_CHECK_CLOSE(m.samples.value<double>("sd(p0)"),0.56,10);
}

class McmcRegress : public Metropolis<McmcRegress,2> {
public:
    
    Array<> x = 10;
    Array<> y = 10;
    
    void reset(void){
        x = {0,1,2,3,4,5,6,7,8,9};
        y = {0,1.9,2.4,3.8,4.9,6.1,7.5,8.9,10.1,11.5};
        values = {0,1};
        variances = {0.01,0.01};
        Metropolis<McmcRegress,2>::reset();
    }

    double log_like(const Array<>& parameters){
        double a = parameters(0);
        double b = parameters(1);
        double ll = 0;
        for(int i=0;i<10;i++){
            double pred = a + x(i)*b;
            ll += std::log(Normal(y(i),0.1).pdf(pred));
        }
        return ll;
    }
};

BOOST_AUTO_TEST_CASE(test_2){
    McmcRegress m;
    m.reset();
    m.run(10000);
}

BOOST_AUTO_TEST_SUITE_END()

} // end namespace Fsl
} // end namespace Estimation
} // end namespace Mcmc
