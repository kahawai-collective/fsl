#pragma once

#include <stencila/array.hpp>
#include <stencila/query.hpp>
using namespace Stencila;

#include <fsl/date.hpp>

#include <fsl/math/functions/function.hpp>

#include <fsl/math/functions/constant.hpp>
using Fsl::Math::Functions::Constant;

#include <fsl/math/functions/identity.hpp>
using Fsl::Math::Functions::Identity;

#include <fsl/math/functions/line.hpp>
using Fsl::Math::Functions::Line;

#include <fsl/math/functions/power.hpp>
using Fsl::Math::Functions::Power;

#include <fsl/math/functions/logistic.hpp>
using Fsl::Math::Functions::Logistic;

#include <fsl/math/functions/double-logistic.hpp>
using Fsl::Math::Functions::DoubleLogistic;

#include <fsl/math/functions/double-normal-plateau.hpp>
using Fsl::Math::Functions::DoubleNormalPlateau;

#include <fsl/math/series/autocorrelation.hpp>
using Fsl::Math::Series::Autocorrelation;

#include <fsl/math/probability/normal.hpp>
using Fsl::Math::Probability::Normal;
using Fsl::Math::Probability::NormalCv;

#include <fsl/math/probability/lognormal.hpp>
using Fsl::Math::Probability::Lognormal;

#include <fsl/math/probability/functional.hpp>
using Fsl::Math::Probability::Functional;

#include <fsl/population/mortality/instantaneous.hpp>

#include <fsl/population/growth/von-bert.hpp>
using Fsl::Population::Growth::VonBert;

#include <fsl/population/morphometry/morphometry.hpp>

#include <fsl/population/maturity/maturity.hpp>

#include <fsl/population/recruitment/autocorrelated.hpp>
using Fsl::Population::Recruitment::Autocorrelated;

#include <fsl/population/recruitment/beverton-holt.hpp>
using Fsl::Population::Recruitment::BevertonHolt;

enum Binary {no=1,yes=1};

template<
    class On,
    class Off
>
class Switch : public On, public Off {
private:
    bool state_;

public:
    operator bool(void) const {
        return state_;
    }

    template<
        class... Args
    >
    double operator()(Args... args){
        return state_?
            On::operator()(args...):
            Off::operator()(args...);
    }

    Switch& on(bool on=true){
        state_ = on;
        return *this;
    }

    Switch& off(bool off=true){
        state_ = not off;
        return *this;
    }
};

namespace Fsl {
namespace Models {

/**
 * Sex, age and sector structured fishery model.
 * 
 * @author Nokome Bentley <nokome.bentley@trophia.com>
 */
template<
    class Derived,
    uint Sexes,
    uint Ages,
    uint Sectors
>
class Matiri : public Polymorph<Derived> {
public:

    Matiri(void){
    }

    /**
     * @name Dimensions
     * @{   
     */
    
    struct Sex : Dimension<Sex,Sexes>{
        Sex(void):Dimension<Sex,Sexes>("sex"){}
        static const char* name(void) { return "sex"; }
    } sexes;

    struct Age : Dimension<Age,Ages>{
        Age(void):Dimension<Age,Ages>("age"){}
        static const char* name(void) { return "age"; }
    } ages;

    struct Sector : Dimension<Sector,Sectors>{
        Sector(void):Dimension<Sector,Sectors>("sector"){}
        static const char* name(void) { return "sector"; }
    } sectors;

    //! @}
    
    /**
     * Fish numbers by age and sex
     */
    Array<double,Sex,Age> numbers = 0;

    /**
     * Fish biomass
     */
    double biomass = 0;

    /**
     * @name Spawning
     * @{
     */

    /**
     * Fish spawning biomass
     */
    double biomass_spawning = 0;

    /**
     * @}
     */

    /**
     * @name Recruitment
     * @{
     */
    
    /**
     * BevertonHolt or Constant recruitment relation
     */
    Switch<BevertonHolt,Constant> recruitment_relation;

    /**
     * Lognormal or Constant recruitment variation
     */
    Switch<Autocorrelated<Lognormal>,Constant> recruitment_variation;

    /**
     * Flag for whether recruiment deviations are drawn randomly (otherwise
     * they may be set in each time by the user)
     */
    bool recruitment_variation_random = true;

    /**
     * Deterministic recruitment at time t 
     */
    double recruits_determ = 0;

    /**
     * Recruitment deviation (multiplier) at time t
     */
    double recruits_deviation = 1;

    /**
     * Total number of recruits at time t
     */
    double recruits = 0;

    
    double sex_ratio = 0.5;

    /**
     * @}
     */ 
    
    /**
     * @name Natural mortality
     * @{
     */
    
    /**
     * Instantaneous rate of natural mortality for each sex
     */
    Array<
        double,
        Sex
    > mortality;

    /**
     * Mortality at sex and age
     */
    Array<
        double,
        Sex,Age
    > mortalities;

    /**
     * Survival at sex and age
     */
    Array<
        double,
        Sex,Age
    > mortality_survivals;

    /**
     * @}
     */
        
    /**
     * @name Weight, maturity at age
     * @{
     */

    /**
     * Length at age
     */
    
    struct LengthAge : VonBert {
        double cv1;
        double cv2;

        Normal distribution(const double& age){
            auto mean = VonBert::operator()(age);
            auto sd = mean * cv1;
            return Normal(mean,sd);
        }
    };

    Array<
        LengthAge,
        Sex
    > length_age;

    Array<
        Normal,
        Sex,Age
    > lengths;

    /**
     * Weight at age
     */
    
    Array<
        Power,
        Sex
    > weight_length;
    
    Array<
        double,
        Sex,Age
    > weights;

    /**
     * Maturity at age
     */
    
    Array<
        Logistic,
        Sex
    > maturity_age;

    Array<
        double,
        Sex,Age
    > maturities;

    /**
     * @}
     */
    
    Array<
        double,
        Sector,Sex
    > selectivity_sex = 1;

    Array<
        DoubleNormalPlateau,
        Sector,Sex
    > selectivity_age;
    
    Array<
        double,
        Sector,Sex,Age
    > selectivities;

    /**
     * A switch used to turn on/off exploitation dynamics
     * (e.g turn off for virgin equilibrium)
     */
    bool exploitation_on = true;

    /**
     * A switch used to turn on/off the calculation
     * of exploitation rates from catches. Used to specify a particular
     * exploitation rate when calculation MSY/Bmsy
     */
    bool catches_on = true;

    /**
     * Vulnerable biomass by sector
     */
    Array<double,Sector> biomass_vulnerable = 0;

    /**
     * Catches by sector
     */
    Array<double,Sector> catches = 0;

    Array<double,Sector> exploitation_rate_max = 1;

    /**
     * Exploitation rate by region and method for current time step
     */
    Array<double,Sector> exploitation_rate = 0;

    /**
     * Exploitation survival by sex and age
     */
    Array<double,Sex,Age> exploitation_survivals = 1;

    /**
     * @}
     */
    
    double biomass_update(void){
        biomass = 0;
        for(auto sex : sexes){
            for(auto age : ages){
                biomass += numbers(sex,age) * weights(sex,age);
            }
        }
        biomass *= 0.001;
        return biomass;
    }
    
    double biomass_spawning_update(void){
        biomass_spawning = 0;
        for(auto sex : sexes){
            for(auto age : ages){
                biomass_spawning += numbers(sex,age) * weights(sex,age) * maturities(sex,age);
            }
        }
        biomass_spawning *= 0.001;
        return biomass_spawning;
    }

    void biomass_vulnerable_update(void){
        for(auto sector : sectors){
            double sum = 0;
            for(auto sex : sexes){
                for(auto age : ages){
                    sum += numbers(sex,age) * weights(sex,age) * selectivities(sector,sex,age);
                }
            }
            biomass_vulnerable(sector) = sum * 0.001;
        }
    }

    void exploitation_off(void){
        exploitation_on = false;
        catches_on = false;
        exploitation_rate = 0;  
    }

    /**
     * Set exploitation rate. Used in testing and in 
     * equilibrium exploitation i.e. MSY/BMSY calculations
     */
    void exploitation_rate_set(const Array<double,Sector>& values){
        exploitation_on = true;
        catches_on = false;
        exploitation_rate = values;
    }

    void catches_set(const Array<double,Sector>& values){
        exploitation_on = true;
        catches_on = true;
        catches = values;
    }

    void check(void) const {
        recruitment_relation.check();
    }
   
    //! @}

    /**
     * Initialise various model variables based on current parameter values
     */
    void initialise(void){

        // Before checking, determine if parameterising by recruitment_relation.s0
        // or by recruitment_relation.r0 and set other accordingly
        bool use_r0 = true;
        if(recruitment_relation.r0>0){
            use_r0 = true;
            recruitment_relation.s0 = 1;
        }
        else if(recruitment_relation.s0>0){
            use_r0 = false;
            recruitment_relation.r0 = 1;
        }
        else {
            throw std::runtime_error(str(boost::format(
                "Either `Matiri::Model::recruitment_relation.s0` <%s> or `Matiri::Model::recruitment_relation.r0` <%s> must be assigned a value greater than 0")
                    %recruitment_relation.s0%recruitment_relation.r0));
        }

        check();

        for(auto sex : sexes){
            for(auto age : ages){
                double age_ = age.index();

                lengths(sex,age) = length_age(sex).distribution(age_);

                weights(sex,age) = lengths(sex,age).integrate(weight_length(sex));

                maturities(sex,age) = maturity_age(sex)(age_);

                mortalities(sex,age) = mortality(sex);
                
                for(auto sector : sectors){
                    selectivities(sector,sex,age) = selectivity_sex(sector,sex) * selectivity_age(sector,sex)(age_);
                }

                mortality_survivals(sex,age) = Population::Mortality::Rate().instantaneous(mortalities(sex,age)).survival();
            }
        }

        // Seed the population with deterministic equilibrium numbers
        seed();
        
        /**
         * The fish population is initialised to an unfished state
         * by iterating with virgin recruitment until it reaches equibrium
         * defined by less than 0.01% change in total biomass.
         */
        // Turn off recruitment relationship, variation and exploitation
        recruitment_relation.off();
        exploitation_on = false;
        // Go to equilibrium
        equilibrium();
        // Turn on recruitment relationship etc again
        recruitment_relation.on();
        exploitation_on = true;

        /**
         * Once the population has converged to unfished equilibrium, the virgin
         * spawning biomass, or the virgin recruitment, can be set.
         */
        if(use_r0){
            recruitment_relation.s0 = biomass_spawning;
        }
        else {
            // Parameterised by B0 so scale everything up
            double scaler = recruitment_relation.s0/biomass_spawning;
            recruitment_relation.r0 *= scaler;
            for(auto sex : sexes){
                for(auto age : ages){
                    numbers(sex,age) *= scaler;
                }
            }
            biomass *= scaler;
            biomass_spawning *= scaler;
        }
    }

    /**
     * Update the model
     */
    void update(uint time=0){

        // Calculate number of recruits
        recruits_determ = recruitment_relation?recruitment_relation(biomass_spawning):recruitment_relation.r0;
        if(recruitment_variation) {
            if(recruitment_variation_random) recruits_deviation = recruitment_variation.random();
        }
        else recruits_deviation = 1;
        recruits = recruits_determ * recruits_deviation;

        // Ageing and recruitment
        for(auto sex : sexes){
            // Oldest age class accumulates 
            numbers(sex,ages.size()-1) += numbers(sex,ages.size()-2);
            // For most ages just "shuffle" along
            for(uint age=ages.size()-2;age>0;age--){
                numbers(sex,age) = numbers(sex,age-1);
            }
            // Recruits are split between sexes according to the sex ratio
            numbers(sex,0) = recruits * sex_ratio;
        }

        // Update biomasses
        biomass_update();
        biomass_spawning_update();
        biomass_vulnerable_update();

        // Exploitation rate
        if(exploitation_on){
            if(catches_on){
                for(auto sector : sectors){
                    double er = biomass_vulnerable(sector)>0?(catches(sector)/biomass_vulnerable(sector)):1;
                    exploitation_rate(sector) = (er>exploitation_rate_max(sector))?exploitation_rate_max(sector):er;
                } 
            }

            for(auto sector : sectors){
                catches(sector) = exploitation_rate(sector) * biomass_vulnerable(sector);
            }

            // Pre-calculate the exploitation_survivals for each sex and age
            for(auto sex : sexes){
                for(auto age : ages){
                    double prod = 1;
                    for(auto sector : sectors){
                        prod *= (1 - exploitation_rate(sector) * selectivities(sector,sex,age));
                    }
                    if(prod<0) prod = 0;
                    exploitation_survivals(sex,age) = prod;
                }
            }
        } else {
            biomass_vulnerable = 0.0;
            exploitation_rate = 0.0;
            exploitation_survivals = 1.0;
        }

        // Mortality and exploitation
        for(auto sex : sexes){
            for(auto age : ages){
                numbers(sex,age) *=  mortality_survivals(sex,age) * exploitation_survivals(sex,age);
            }
        }
    }

    void seed(void){
        // Seed the numbers at age
        for(auto sex : sexes){
            double surviving = recruitment_relation.r0 * ((sex==0)?sex_ratio:(1-sex_ratio));
            for(auto age : ages){
                surviving *= mortality_survivals(sex,age);
                numbers(sex,age) = surviving;
            }
        }
    }

    /**
     * Move the population to a deterministic equilibrium 
     */
    void equilibrium(void){
        // Turn off recruitment variation
        bool recruitment_variation_on = recruitment_variation;
        recruitment_variation.off();
        // Iterate until there is a very minor change in biomass
        uint steps = 0;
        const uint steps_max = 10000;
        double biomass_prev = 1;
        while(steps<steps_max){
            update();

            double diff = fabs(biomass-biomass_prev)/biomass_prev;
            if(diff<0.00001 and steps>ages.size()) break;
            biomass_prev = biomass;

            steps++;
        }
        // Throw an error if there was no convergence
        if(steps>steps_max) throw std::runtime_error("Did not converge");
        // Turn on recruitment deviation again
        if(recruitment_variation_on) recruitment_variation.on();
    }
}; // class Matiri

class MatiriWriter {
public:

    std::ofstream overall;
    std::ofstream sex;
    std::ofstream sex_age;
    std::ofstream sector;
    std::ofstream sector_sex_age;

    MatiriWriter(const std::string& path="."){
        overall.open(path+"/overall.tsv");
        overall<<"tag\ttime\tbiomass\tbiomass_spawning\trecruits_determ\trecruits_deviation\trecruits\tsex_ratio\n";

        sex.open(path+"/sex.tsv");
        sex<<"tag\ttime\tsex\tmortality\tmaturity_age_inflection\tmaturity_age_steepness\n";

        sex_age.open(path+"/sex_age.tsv");
        sex_age<<"tag\ttime\tsex\tage\tnumbers\tmortalities\tmortality_survivals\tlength_mean\tlength_sd\tweights\tmaturities\n";

        sector.open(path+"/sector.tsv");
        sector<<"tag\ttime\tsector\tbiomass_vulnerable\tcatches\texploitation_rate_max\texploitation_rate\n";

        sector_sex_age.open(path+"/sector_sex_age.tsv");
        sector_sex_age<<"tag\ttime\tsector\tsex\tage\tselectivities\n";            
    }

    template<class Model,typename Tag>
    void write(const Model& model, const Tag& tag, uint time=0){
        this->overall
            <<tag<<"\t"
            <<time<<"\t"
            <<model.biomass<<"\t"
            <<model.biomass_spawning<<"\t"
            <<model.recruits_determ<<"\t"
            <<model.recruits_deviation<<"\t"
            <<model.recruits<<"\t"
            <<model.sex_ratio<<"\n";

        for(auto sex : model.sexes){
            this->sex
                <<tag<<"\t"<<time<<"\t"<<sex<<"\t"
                <<model.mortality(sex)<<"\t"
                <<model.length_age(sex).k<<"\t"
                <<model.length_age(sex).linf<<"\t"
                <<model.length_age(sex).cv1<<"\t"
                <<model.length_age(sex).cv2<<"\t"
                <<model.weight_length(sex).a<<"\t"
                <<model.weight_length(sex).b<<"\t"
                <<model.maturity_age(sex).inflection<<"\t"
                <<model.maturity_age(sex).steepness<<"\n"
                ;
        }
        for(auto sex : model.sexes){
            for(auto age : model.ages){
                this->sex_age
                    <<tag<<"\t"<<time<<"\t"<<sex<<"\t"<<age<<"\t"
                    <<model.numbers(sex,age)<<"\t"
                    <<model.mortalities(sex,age)<<"\t"
                    <<model.mortality_survivals(sex,age)<<"\t"
                    <<model.lengths(sex,age).mean()<<"\t"
                    <<model.lengths(sex,age).sd()<<"\t"
                    <<model.weights(sex,age)<<"\t"
                    <<model.maturities(sex,age)<<"\n"
                    ;
            }
        }
        for(auto sector : model.sectors){
            this->sector
                <<tag<<"\t"<<time<<"\t"<<sector<<"\t"
                <<model.biomass_vulnerable(sector)<<"\t"
                <<model.catches(sector)<<"\t"
                <<model.exploitation_rate_max(sector)<<"\t"
                <<model.exploitation_rate(sector)<<"\n"
                ;
        }
        for(auto sector : model.sectors){
            for(auto sex : model.sexes){
                for(auto age : model.ages){
                    this->sector_sex_age
                        <<tag<<"\t"<<time<<"\t"<<sector<<"\t"<<sex<<"\t"<<age<<"\t"
                        <<model.selectivities(sector,sex,age)<<"\n";
                }
            }
        }
    }

    template<class Model>
    void write(Model& model, const std::string& tag, uint begin, uint end){
        for(uint time=begin;time<end;time++){
            model.update(time);
            write(model,tag,time);
        }
    }

}; // class MatiriWriter

} // namespace Models
} // namespace Fsl
