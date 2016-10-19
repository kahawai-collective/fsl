#pragma once

#include <fsl/common.hpp>

#include <fsl/population/sex-age.hpp>
#include <fsl/harvesting/sex-age.hpp>

namespace Fsl {
namespace Monitoring {

class DistributionSummary : public Structure <DistributionSummary> {
public:

	double mean;
	double median;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(mean, "mean")
            .data(median, "median")
        ;
    }

    template<class Dimension>
    void calculate(Array<double, Dimension> array) {
    	double cumul = 0;
    	double sum = 0;
    	double sumprod = 0;
    	uint n = 0;
    	median = 0;
    	for (auto level : Dimension::levels) {
    		auto value = level.index();
    		auto prop = array(level);
    		cumul += prop;
    		sum += prop;
    		sumprod += prop * value;
    		if (median == 0 and cumul >= 0.5) median = value;
    		n++;
    	}
    	mean = sumprod/sum;
    }

};

template<class Time, class Length>
class LengthCatchSampling : public Structure< LengthCatchSampling<Time, Length> > {
public:

	Array<double, Time, Length> proportions;
	Array<DistributionSummary, Time> summaries;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(proportions, "proportions")
            .data(summaries, "summaries")
        ;
    }

	void initialise() {

	}

	template<class Sex, class Age>
	void update(uint time, const Population::SexAge<Sex, Age>& population, const Harvesting::SexAge<Sex, Age>& harvesting) {
		Array<double, Length> sample = 0;
		double sum = 0;
		for (auto length : Length::levels) {
			double l = length.index();
			for (auto sex : Sex::levels) {
				for (auto age : Age::levels) {
					sample(length) += population.numbers(sex, age) * 
									population.lengths(sex, age).integral(l, l+1) * 
									harvesting.selectivities(sex, age);
				}
			}
			sum += sample(length);
		}
		sample /= sum;
		for (auto length : Length::levels) proportions(time, length) = sample(length);
		summaries(time).calculate(sample);
	}

};

}
}
