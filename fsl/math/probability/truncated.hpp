#pragma once

namespace Fsl {
namespace Math {
namespace Probability {
	
template<typename Base>
class Truncated : public Base {
public:
	double min;
	double max;
	
	Truncated(double a = NAN,double b = NAN,double min_ = -INFINITY, double max_ = INFINITY):
		Base(a,b),min(min_),max(max_){}

	double minimum(void) const {
		return min;
	}

	double maximum(void) const {
		return max;
	}

	double random(void) const {
		double trial = Base::random();
		if(trial<min or trial>max) return random();
		else return trial;
	};
};

}}}
