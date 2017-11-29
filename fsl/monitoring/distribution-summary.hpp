#pragma once

#include <fsl/common.hpp>

namespace Fsl {
namespace Monitoring {

class DistributionSummary : public Structure <DistributionSummary> {
public:

	double mean = NAN;
	double median = NAN;
    double p10 = NAN;
    double mrad = NAN;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(mean, "mean")
            .data(median, "median")
            .data(p10, "p10")
            .data(mrad, "mrad")
        ;
    }

    template<class Dimension>
    void calculate(const Array<double, Dimension>& array) {
    	double cumul = 0;
    	double sum = 0;
    	double sumprod = 0;
        double last = -1;
    	uint n = 0;
    	median = 0;
        p10 = 0;
        mrad = 0;
    	for (auto level : Dimension::levels) {
    		auto value = level.index();
    		auto prop = array(level);
    		cumul += prop;
    		sum += prop;
    		sumprod += prop * value;
    		if (median == 0 and cumul >= 0.5) median = value;
            if (p10 == 0 and cumul >= 0.1) p10 = value;
            if (last >=0.01 and prop>=0.01) {
                mrad += std::fabs(prop-last)/(prop+last)*0.5;
            }
            last = prop;

    		n++;
    	}
    	mean = sumprod/sum;
        mrad /=n;
    }

};

}
}
