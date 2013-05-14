#pragma once

#include <fsl/property.hpp>

namespace Fsl {
namespace Population {
namespace Mortality {

/*!
A simple mortality rate

This class is for modelling a simple mortality rate.
It provides convienince function for getting and setting the
instantaneous rates of mortality and getting survival.
*/
class Rate {
private:

    double rate_;

public:

    FSL_PROPERTY_DEF(Rate,rate,double)
    
    //! Get the rate of mortality (\f$r\f$)
    double rate(void) const {
        return rate_;
    };

    //! Set the rate of mortality (\f$r\f$)
    Rate& rate(const double& rate) {
        rate_ = rate;
        return *this;
    };
    
    FSL_PROPERTY_DEF(Rate,instantaneous,double)

    //! Get the instantaneous rate of mortality (\f$i=-ln(1-r)\f$)
    double instantaneous() const {
        return -std::log(1-rate_);
    }

    //! Set the instantaneous rate of mortality (\f$r=1-exp(-i)\f$)
    Rate& instantaneous(const double& i){
        rate_ = 1-std::exp(-i);
        return *this;
    }
    
    FSL_PROPERTY_DEF(Rate,survival,double)

    //! Get the survival rate (\f$1-r\f$)
    double survival(void) const {
        return 1-rate_;
    }

    //! Set the survival rate (\f$1-r\f$)
    Rate& survival(const double& s) {
        rate_ = 1-s;
        return *this;
    }
    
    Rate& initialise(void){
        return *this;
    }
};

}
}
}
