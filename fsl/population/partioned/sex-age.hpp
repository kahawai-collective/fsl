#pragma once

#include <stencila/structure.hpp>
using Stencila::Structure;

#include <stencila/array.hpp>
using Stencila::Array;

#include <stencila/query.hpp>
using Stencila::sum;

#include <fsl/population/recruitment/beverton-holt.hpp>
using Fsl::Population::Recruitment::BevertonHolt;

#include <fsl/population/growth/von-bert.hpp>
using Fsl::Population::Growth::VonBert;


#include <fsl/math/probability/normal.hpp>
using Fsl::Math::Probability::Normal;
using Fsl::Math::Probability::NormalCv;

#include <fsl/math/probability/lognormal.hpp>
using Fsl::Math::Probability::Lognormal;


#include <fsl/math/functions/power.hpp>
using Fsl::Math::Functions::Power;

#include <fsl/math/functions/logistic.hpp>
using Fsl::Math::Functions::Logistic;


namespace Fsl {
namespace Population {
namespace Partioned {

template<class Sexes, class Ages>
class SexAge : public Structure< SexAge<Sexes, Ages> > {
  public:

    const int age_max = Ages::levels.size();

    /**
     * @name State
     * @{
     */
    
    /**
     * Numbers by sex and age
     */
    Array<double, Sexes, Ages> numbers = 0;

    /**
     * @}
     */


    /**
     * @name Recruitment
     * @{
     */
 
    /**
     * BevertonHolt stock-recruitment relation
     */
    BevertonHolt stock_recruits;

    /**
     * Lognormal recruitment variation
     */
    Lognormal recruits_variation = {1, 0.6};

    /**
     * Spawning biomass at last update
     */
    double spawning_biomass = 0;

    /**
     * Should the number of recruits be related to the number of spawners?
     *
     * This flag is used to turn off the stock-recruit relation when 
     * taking the stock to deterministic pristine equilibrium
     */
    bool recruits_related = true;

    /**
     * Should the number of recruits vary around the deterministic level
     *
     * This flag is used to turn off recruitment variability when we want
     * to simulate deterministric, equilibrium dynamics.
     */
    bool recruits_vary = true;

    /**
     * Deterministic recruitment at last update
     */
    double recruits_determ = 0;

    /**
     * Recruitment deviation (multiplier) at last update
     */
    double recruits_deviation = 1;

    /**
     * Total number of recruits at last update
     */
    double recruits = 0;


    /**
     * Sex ratio at birth
     */
    double sex_ratio = 0.5;

    /**
     * @}
     */
    

    /**
     * @name Natural mortality
     * @{
     */

    /**
     * Constant natural mortality by sex
     */
    Array<double, Sexes> mortality_sex = 0.1;

    /**
     * Survivals at sex and age
     */
    Array<double, Sexes, Ages> mortalities;

    /**
     * @}
     */
    

    /**
     * @name Length at age
     * @{
     */

    /**
     * Length at age relation. vonBertallanfy with a normal distribution
     */
    struct LengthAge : VonBert {
        double cv;

        Normal distribution(const double& age){
            auto mean = value(age);
            auto sd = mean * cv;
            return Normal(mean,sd);
        }

    };

    /**
     * Lengths at age relations for each sex
     */
    Array<LengthAge, Sexes> length_age;

    /**
     * Length at age distibutions for each sex and age
     */
    Array<Normal, Sexes, Ages> lengths;

    /**
     * @}
     */
        

    /**
     * @name Weight at age
     * @{
     */

    /**
     * Weight at age reltion for each sex
     */
    Array<Power, Sexes> weight_length;
    
    /**
     * Mean weight at age for each sex
     */
    Array<double, Sexes, Ages> weights;

    /**
     * @}
     */


    /**
     * @name Maturity at age
     * @{
     */
    
    /**
     * Maturity at age relation for each sex
     */
    Array<Logistic, Sexes> maturity_age;

    /**
     * Proportion mature by sex and age
     */
    Array<double, Sexes, Ages> maturities;

    /**
     * @}
     */


    /**
     * Reflection
     */
    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(numbers, "numbers")
            .data(stock_recruits, "stock_recruits")
            .data(sex_ratio, "sex_ratio")
            .data(mortality_sex, "mortality_sex")
            .data(mortalities, "mortalities")
            .data(length_age, "length_age")
            .data(lengths, "lengths")
            .data(weight_length, "weight_length")
            .data(weights, "weights")
        ;
    }

    /**
     * Initialise the model
     */
    void initialise(void) {
        for (auto sex : Sexes::levels) {
            for (auto age : Ages::levels) {
                auto years = age.index() + 0.5;

                lengths(sex, age) = length_age(sex).distribution(years);

                weights(sex, age) = lengths(sex, age).integrate(
                    weight_length(sex)
                );

                maturities(sex, age) = maturity_age(sex).value(years);

                mortalities(sex, age) = mortality_sex(sex);
            }
        }
    }

    /**
     * Update
     */
    void update(void) {

        // Spawning biomass
        spawning_biomass = biomass_spawning();

        // Recruits
        recruits_determ = recruits_related ? stock_recruits(spawning_biomass) : stock_recruits.r0;
        recruits_deviation = recruits_vary ? recruits_variation.random() : 1;
        recruits = recruits_determ * recruits_deviation;

        // Ageing and recruitment
        for(auto sex : Sexes::levels){
            // Oldest age class accumulates 
            numbers(sex, age_max) += numbers(sex, age_max-1);
            // For most ages just "shuffle" along
            for(uint age = age_max-1; age > 0; age--){
                numbers(sex, age) = numbers(sex, age-1);
            }
            // Recruits are split between sexes according to the sex ratio
            numbers(sex, 0) = recruits * sex_ratio;
        }
    }

    void seed(void){
        // Seed the numbers at age
        for (auto sex : Sexes::levels) {
            double surviving = stock_recruits.r0 * ((sex==0)?sex_ratio:(1-sex_ratio));
            for (auto age : Ages::levels) {
                surviving *= mortality_survivals(sex, age);
                numbers(sex, age) = surviving;
            }
        }
    }

    /**
     * Move the population to a deterministic equilibrium 
     */
    void equilibrium(void){
        // Turn off recruitment variation
        recruits_vary = false;
        // Iterate until there is a very minor change in spawning_biomass
        uint steps = 0;
        const uint steps_max = 10000;
        double spawning_biomass_prev = 1;
        while(steps<steps_max){
            update();

            double diff = fabs(spawning_biomass-spawning_biomass_prev)/spawning_biomass_prev;
            if(diff<0.00001 and steps>ages.size()) break;
            spawning_biomass_prev = spawning_biomass;

            steps++;
        }
        // Throw an error if there was no convergence
        if(steps>steps_max) throw std::runtime_error("Did not converge");
        // Turn on recruitment variation again
        recruits_vary = true;
    }

    /**
     * Total numbers
     */
    double numbers_total(void){
        return sum(numbers);
    }

    /**
     * Total biomass
     */
    double biomass_total(void){
        double biomass = 0;
        for(auto sex : Sexes::levels){
            for(auto age : Ages::levels){
                biomass += numbers(sex, age) * weights(sex, age);
            }
        }
        biomass *= 0.001;
        return biomass;
    }

    /**
     * Spawning biomass
     */
    double biomass_spawning(void){
        double biomass = 0;
        for(auto sex : Sexes::levels){
            for(auto age : Ages::levels){
                biomass += numbers(sex, age) * weights(sex, age) * maturity(sex, age);
            }
        }
        biomass *= 0.001;
        return biomass;
    }

};

}
}
}
