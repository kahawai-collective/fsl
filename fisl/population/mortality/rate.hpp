#pragma once

namespace Fisl {
namespace Population {
namespace Mortality {

/*!
A simple mortality rate

This class is for modelling a simple mortality rate.
It provides convienince function for getting and setting the
instantaneous rates of mortality and getting survival.
*/
class Rate {

	//! Rate of mortality (\f$r\f$)
	double rate;

	//! Get the instantaneous rate of mortality (\f$i=-ln(1-r)\f$)
	double instantaneous() const {
		return -std::log(1-rate);
	}
	
	//! Set the instantaneous rate of mortality (\f$r=1-exp(-i)\f$)
	void instantaneous(const double& i){
		rate = 1-std::exp(-i);
	}

	//! Get the survival rate (\f$1-r\f$)
	Real survival(void) const {
		return 1-rate;
	}
};

}
}
}
