#pragma once

#include <fsl/common.hpp>

#include <fsl/population/sex-age.hpp>
#include <fsl/harvesting/sex-age.hpp>
#include <fsl/monitoring/distribution-summary.hpp>

namespace Fsl {
namespace Monitoring {

template<class Time, class Length>
class LengthCatchSampling : public Structure< LengthCatchSampling<Time, Length> > {
public:

	Array<double, Time, Length> proportions;
	Array<DistributionSummary, Time> summaries;

	/**
	 * An array of the proportion of fish of each sex and age in 
	 * each length bin. See initialias() and update() for it's usage
	 */
	Array<double> fractions;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(proportions, "proportions")
            .data(summaries, "summaries")
        ;
    }

	template<class Sex, class Age>
	void initialise(const Population::SexAge<Sex, Age>& population) {
		fractions.size(Sex::size() * Age::size() * Length::size());
		uint index = 0;
		for (auto length : Length::levels) {
			double l = length.index();
			for (auto sex : Sex::levels) {
				for (auto age : Age::levels) {
					fractions[index] = population.lengths(sex, age).integral(l, l+1);
					index++;
				}
			}
		}
	}

	template<class Sex, class Age>
	void update(uint time, const Population::SexAge<Sex, Age>& population, const Harvesting::SexAge<Sex, Age>& harvesting) {
		Array<double, Length> sample = 0;
		double sum = 0;
		uint index = 0;
		for (auto length : Length::levels) {
			for (auto sex : Sex::levels) {
				for (auto age : Age::levels) {
					sample(length) += population.numbers(sex, age) * 
									fractions[index] * 
									harvesting.selectivities(sex, age);
					index++;
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
