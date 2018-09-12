#pragma once

#include <stencila/structure.hpp>
using Stencila::Structure;

#include <stencila/array.hpp>
using Stencila::Array;

#include <fsl/math/functions/double-normal-plateau.hpp>
using Fsl::Math::Functions::DoubleNormalPlateau;

namespace Fsl {
namespace Harvesting {

template<class Sexes, class Ages>
class SexAge : public Structure< SexAge<Sexes, Ages> > {
  public:

    const Sexes sexes = Sexes::levels;
    const Ages ages = Ages::levels;

    /**
     * @name Harvest
     * @{
     */
    
    double quantity = 0;
    double rate = 0;

    double rate_max = 0.9999;    

    double biomass_selected_last = 0;
    /**
     * @}
     */

 
    /**
     * @name Selectivities
     * @{
     */

    /**
     * Selectivity curve by sex
     */
    Array<DoubleNormalPlateau, Sexes> selectivity_age;

    /**
     * Survivals at sex and age
     */
    Array<double, Sexes, Ages> selectivities;

    /**
     * @}
     */


    /**
     * Reflection
     */
    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(selectivity_age, "selectivity_age")
            .data(selectivities, "selectivities")
        ;
    }

    /**
     * Initialise
     */
    void initialise(void) {
        for (auto sex : Sexes::levels) {
            for (auto age : Ages::levels) {
                auto years = age.index() + 0.5;
                selectivities(sex, age) = selectivity_age(sex).value(years);
            }
        }
    }

    /**
     * Update
     */
    template<class Population>
    void update(unsigned int time, Population* population) {
        biomass_selected_last = biomass_selected(*population);
        rate = quantity/biomass_selected_last;
        if (rate > rate_max) {
            rate = rate_max;
            quantity = rate * biomass_selected_last;
        }
        for(auto sex : sexes){
            for(auto age : ages){
                population->numbers(sex, age) *= 1 - selectivities(sex, age) * rate;
            }
        }       
    }

    /**
     * Selected biomass
     */
    template<class Population>
    double biomass_selected(const Population& population) const {
        double biomass = 0;
        for(auto sex : sexes){
            for(auto age : ages){
                biomass += population.numbers(sex, age) * population.weights(sex, age) * selectivities(sex, age);
            }
        }
        biomass *= 0.001;
        return biomass;
    }

};

}
}
