#pragma once

namespace Fisl {

class Real {
private:
	double value;
public:
	Real(const double& value_=0)
		:value(value_){
	}
	void operator=(const double& value_){
		value = value_;
	}
	operator double(void) const {
		return value;
	}
	Real& operator()(void){
		return *this;
	}
	Real& operator()(const double& value_){
		value = value_;
		return *this;
	}
};

}
