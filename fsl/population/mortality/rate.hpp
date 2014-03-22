#pragma once

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

    Rate(const double& rate=0):
        rate_(rate){
    }

    operator double(void) const {
        return rate();
    }
    
    //! Get the rate of mortality (\f$r\f$)
    double rate(void) const {
        return rate_;
    }

    //! Set the rate of mortality (\f$r\f$)
    Rate& rate(const double& rate) {
        rate_ = rate;
        return *this;
    }

    double instantaneous(void) const {
        return -std::log(1-rate_);
    }

    Rate& instantaneous(const double& value){
        rate_ = 1-std::exp(-value);
        return *this;
    }
    
    //! Get the survival rate (\f$1-r\f$)
    double survival(void) const {
        return 1-rate_;
    }

    //! Set the survival rate (\f$1-r\f$)
    Rate& survival(const double& s) {
        rate_ = 1-s;
        return *this;
    }    
};

}
}
}
