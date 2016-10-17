#pragma once

#include <stencila/structure.hpp>
using Stencila::Structure;

#include <stencila/array.hpp>
using Stencila::Array;

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

    /**
     * @name State
     * @{
     */
    
    /**
     * Numbers by sex and age
     */
    Array<double, Sexes, Ages> numbers;

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
    BevertonHolt stock_recruitment;


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
    Array<double, Sexes> mortality_sex;

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
            .data(stock_recruitment, "stock_recruitment")
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
        for (auto sex : Sexes::levels()) {
            for (auto age : Ages::levels()) {

                lengths(sex,age) = length_age(sex).distribution(age);

                weights(sex,age) = lengths(sex,age).integrate(
                    weight_length(sex)
                );

                maturities(sex,age) = maturity_age(sex).value(age);

                mortalities(sex,age) = mortality_sex(sex);
            }
        }
    }

};

}
}
}
