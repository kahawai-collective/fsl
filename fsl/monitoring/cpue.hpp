#pragma once

#include <fsl/common.hpp>

namespace Fsl {
namespace Monitoring {

template<class Time>
class Cpue : public Structure< Cpue<Time> > {
public:

	Array<double, Time> series;

	template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(series, "series")
        ;
    }

	void initialise() {
	}

	template<class Population, class Harvesting>
	void update(uint time, const Population& population, const Harvesting& harvesting) {
		series(time) = harvesting.biomass_selected(population);
	}
};

}
}
