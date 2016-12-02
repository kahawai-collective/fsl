#pragma once

#include <fsl/common.hpp>

namespace Fsl {
namespace Monitoring {

class DistributionSummary : public Structure <DistributionSummary> {
public:

	double mean;
	double median;
    double p10;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(mean, "mean")
            .data(median, "median")
            .data(p10, "p10")
        ;
    }

    template<class Dimension>
    void calculate(Array<double, Dimension> array) {
    	double cumul = 0;
    	double sum = 0;
    	double sumprod = 0;
    	uint n = 0;
    	median = 0;
        p10 = 0;
    	for (auto level : Dimension::levels) {
    		auto value = level.index();
    		auto prop = array(level);
    		cumul += prop;
    		sum += prop;
    		sumprod += prop * value;
    		if (median == 0 and cumul >= 0.5) median = value;
            if (p10 == 0 and cumul >= 0.1) p10 = value;
    		n++;
    	}
    	mean = sumprod/sum;
    }

};

}
}
