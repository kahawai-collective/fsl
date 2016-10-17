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

    const unsigned int age_max = Ages::levels.size();

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
    double biomass_spawning_last = 0;

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
     * @name Exploitaitons
     * @{
     */
    
    bool exploitation_on = true;

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
        biomass_spawning_last = biomass_spawning();

        // Recruits
        recruits_determ = recruits_related ? stock_recruits(biomass_spawning_last) : stock_recruits.r0;
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
            // Recruits are split evenly between sexes
            numbers(0, 0) = recruits * 1.0/(Sexes::levels.size());
        }
    }

    void seed(void){
        // Seed the numbers at age
        auto sex_ratio = 1/Sexes::levels.size();
        for (auto sex : Sexes::levels) {
            double surviving = stock_recruits.r0 * sex_ratio;
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
        // Iterate until there is a very minor change in biomass_spawning_last
        uint steps = 0;
        const uint steps_max = 10000;
        double biomass_spawning_prev = 1;
        while(steps<steps_max){
            update();

            double diff = fabs(biomass_spawning_last-biomass_spawning_prev)/biomass_spawning_prev;
            if(diff<0.000001 and steps>age_max) break;
            biomass_spawning_prev = biomass_spawning_last;

            steps++;
        }
        // Throw an error if there was no convergence
        if(steps>steps_max) throw std::runtime_error("Did not converge");
        // Turn on recruitment variation again
        recruits_vary = true;
    }

    void pristine(void){
        /**
         * The fish population is initialised to an unfished state
         * by iterating with virgin recruitment until it reaches equibrium
         * defined by less than 0.01% change in total biomass.
         */
        
        stock_recruits.r0 = 1;

        // Turn off recruitment relationship and exploitation
        recruits_related = false;
        exploitation_on = false;
        // Go to equilibrium
        equilibrium();
        // Turn on recruitment relationship and exploitation again
        recruits_related = true;
        exploitation_on = true;

        /**
         * Once the population has converged to unfished equilibrium, the virgin
         * spawning biomass, or the virgin recruitment, can be set.
         */
        // Parameterised by B0 so scale everything up
        double scaler = stock_recruits.s0/biomass_spawning_last;
        stock_recruits.r0 *= scaler;
        for(auto sex : Sexes::levels){
            for(auto age : Ages::levels){
                numbers(sex, age) *= scaler;
            }
        }
        biomass_spawning_last *= scaler;
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
                biomass += numbers(sex, age) * weights(sex, age) * maturities(sex, age);
            }
        }
        biomass *= 0.001;
        return biomass;
    }

};

}
}
}
