#pragma once

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>

namespace Fsl {
namespace Math {
namespace Statistics {
namespace Univariate {
    
//! @todo temporary
typedef double Real;

using namespace boost::accumulators;

#define LOCAL(NAME,FUNC)\
	class NAME {\
	private:\
		accumulator_set<double, stats<tag::FUNC>> accumulator;\
	public:\
		void append(const double& value){\
			accumulator(value);\
		}\
		Real value(void) const {\
			return FUNC(accumulator);\
		}\
	};

LOCAL(Mean,mean)
LOCAL(Variance,variance)

#undef LOCAL

class Geomean {
private:
	accumulator_set<double, stats<tag::mean>> accumulator;

public:
	void append(const double& value){
		accumulator(std::log(value));
	}

	Real value(void) const {
		return std::exp(mean(accumulator));
	}
};

class MeanAbsoluteProportionChange {
private:
	double last;
	accumulator_set<double, stats<tag::mean>> accumulator;

public:
	MeanAbsoluteProportionChange(void):
		last(0){
	}

	void append(const double& value){
		if(last!=0){
			double proportional_change = std::fabs(value-last)/last;
			accumulator(proportional_change);
		}
		last = value;
	}

	Real value(void) const {
		return mean(accumulator);
	}

};

}
}
}
}
