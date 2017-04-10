#pragma once

#include <fsl/common.hpp>

#include <fsl/math/series/autocorrelation.hpp>
using Fsl::Math::Series::Autocorrelation;

#include <fsl/math/probability/normal.hpp>


namespace Fsl {
namespace Monitoring {

template<class Time>
class Cpue : public Structure< Cpue<Time> > {
public:

	Array<double, Time> series;

	double error = 0;
	double autocorr = 0;
	double beta = 1;
	double trend = 0;

	double catchability = 1;
    Normal error_dist;
    Autocorrelation error_autocorr;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(series, "series")
        ;
    }

	void initialise() {
		catchability = 1;
		error_dist = Normal(0, error);
		error_autocorr = Autocorrelation(autocorr);
	}

	template<class Population, class Harvesting>
	void update(uint time, const Population& population, const Harvesting& harvesting) {
		double cpue_biomass = harvesting.biomass_selected(population);

        double cpue_apparent = std::pow(cpue_biomass, beta);

        // Allow for changes in power
        catchability *= 1 + trend;
        double cpue_acheived = cpue_apparent * catchability;

        // Add autocorrelated error
        double cpue_observed = cpue_acheived * std::exp(error_autocorr(error_dist.random()));

		series(time) = cpue_observed;
	}
};

}
}
