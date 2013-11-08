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
    
    //! Get the rate of mortality (\f$r\f$)
    double rate(void) const {
        return rate_;
    };

    //! Set the rate of mortality (\f$r\f$)
    Rate& rate(const double& rate) {
        rate_ = rate;
        return *this;
    };
    
    class Instantaneous {
    private:
        Rate& parent;
    public:
        Instantaneous(Rate& rate):
            parent(rate){
        }
        void operator=(const double& value){
            parent.rate(1-std::exp(-value));
        }
        operator double(void)const {
            return -std::log(1-parent.rate());
        }
    };
    Instantaneous instantaneous;
    
    //! Get the survival rate (\f$1-r\f$)
    double survival(void) const {
        return 1-rate_;
    }

    //! Set the survival rate (\f$1-r\f$)
    Rate& survival(const double& s) {
        rate_ = 1-s;
        return *this;
    }
    
    Rate(void):
        instantaneous(*this){
        
    }
    
    Rate& initialise(void){
        return *this;
    }
    
};

}
}
}
