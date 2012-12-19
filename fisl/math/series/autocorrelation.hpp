#pragma once

namespace Fisl {
namespace Math {
namespace Series {
	
class Autocorrelation {
private:

	double coefficient_;
	bool started_;
	double last_;
	
public:
	Autocorrelation(void):
		coefficient_(0),
		started_(false),
		last_(0){
	}
	
	double coefficient(void) const {
		return coefficient_;
	}
	
	Autocorrelation& coefficient(double const& value) {
		coefficient_ = value;
		return *this;
	}
	
	double value(const double& deviation) {
		if(started_) {
			last_ = coefficient_*last_ + std::sqrt(1-std::pow(coefficient_,2))*deviation;
		}
		else {
			last_ = deviation;
			started_ = true;
		}
		return last_;
	}
};

}
}
}

