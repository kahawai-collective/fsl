#pragma once

#include <fsl/date.hpp>

#include <fsl/math/functions/function.hpp>

#include <fsl/math/functions/line.hpp>
using Fsl::Math::Functions::Line;

#include <fsl/math/functions/logistic.hpp>
using Fsl::Math::Functions::Logistic;

#include <fsl/math/functions/double-logistic.hpp>
using Fsl::Math::Functions::DoubleLogistic;

#include <fsl/math/functions/double-normal-plateau.hpp>
using Fsl::Math::Functions::DoubleNormalPlateau;

#include <fsl/math/series/autocorrelation.hpp>
using Fsl::Math::Series::Autocorrelation;

#include <fsl/math/probability/normal.hpp>
using Fsl::Math::Probability::NormalCv;

#include <fsl/math/probability/lognormal.hpp>
using Fsl::Math::Probability::Lognormal;

#include <fsl/math/probability/functional.hpp>
using Fsl::Math::Probability::Functional;

#include <fsl/population/mortality/rate.hpp>

#include <fsl/population/growth/von-bert.hpp>
using Fsl::Population::Growth::VonBert;

#include <fsl/population/morphometry/morphometry.hpp>

#include <fsl/population/maturity/maturity.hpp>

#include <fsl/population/recruitment/stochastic.hpp>
using Fsl::Population::Recruitment::Stochastic;

#include <fsl/population/recruitment/beverton-holt.hpp>
using Fsl::Population::Recruitment::BevertonHolt;


namespace Fsl {
namespace Models {
namespace Matiri {

template<
    unsigned int Sexes,
    unsigned int Ages,
    unsigned int Fleets
>
class Model {
public:

    const static unsigned int sexes = Sexes;
    const static unsigned int ages = Ages;
    const static unsigned int fleets = Fleets;
    
    static const char male = 0;
    static const char female = 1;
    
    class Fish;
    class Fishing;

    class Fish {
    public:
        
        Stochastic<
            BevertonHolt,
            Lognormal,
            Autocorrelation
        > stock_recruit;
        
        double sex_ratio;
        
        class Sex {
        public:
            typedef std::array<double,Ages> Numbers;
            Numbers numbers;
            
            typedef Math::Probability::Functional<
                NormalCv,
                VonBert,
                Line,
                Ages
            > Sizes;
            Sizes sizes;
            
            typedef Math::Functions::Cached<Population::Morphometry::Power,Ages> Weights;
            Weights weights;
            
            typedef Math::Functions::Cached<Population::Maturity::Logistic,Ages> Maturities;
            Maturities maturities;
            
            typedef Population::Mortality::Rate Mortalities;
            Mortalities mortalities;
            
            Sex& initialise(void){
                sizes.initialise();
                weights.initialise_integrate(sizes);
                maturities.initialise_integrate(sizes);
                mortalities.initialise();
                return *this;
            }
            
            Sex& set(const double& value){
                auto& self = *this;
                
                for(unsigned int age=0;age<Ages;age++) numbers[age] = value;
                
                return self;
            }
            
            Sex& ageing(const double& recruits){
                numbers[Ages-1] += numbers[Ages-2];
                for(int age=Ages-2;age>0;age--) numbers[age] = numbers[age-1];
                numbers[0] = recruits;
                return *this;
            }
            
            Sex& mortality(void){
                auto& self = *this;
                
                auto survival = mortalities.survival();
                for(unsigned int age=0;age<Ages;age++) numbers[age] *= survival;
                
                return self;
            }
            
            Sex& exploitation(const Date& date,const typename Fishing::Fleet& fleet,const typename Fishing::Fleet::Sex& fleet_sex){
                auto& self = *this;
                
                for(unsigned int age=0;age<Ages;age++) {
                    numbers[age] *= 1 - (fleet.exploitation_rate * fleet_sex.selectivities[age]);
                }
                
                return self;
            }

            double recruits(void) const {
                return numbers[0];
            }
            
            //! @{
            //! @name Biomass calculation methods
            //! @brief These methods return biomass in metric tonnes
            
            double biomass(void) const {
                double biomass = 0;
                for(unsigned int age=0;age<Ages;age++) biomass += numbers[age] * weights[age];
                return biomass/1000;
            }
            
            double biomass_mature(void) const {
                double biomass = 0;
                for(unsigned int age=0;age<Ages;age++) biomass += numbers[age] * weights[age] * maturities[age];
                return biomass/1000;
            }
            
            template<
                class Selectivity
            >
            double biomass_selected(const Selectivity& selectivity) const {
                double biomass = 0;
                for(unsigned int age=0;age<Ages;age++) biomass += numbers[age] * weights[age] * selectivity[age];
                return biomass/1000;
            }
            
            //! @}
        };
        std::array<Sex,2> sexes;
        
        void initialise(const Fishing& fishing){
            stock_recruit.initialise();
            
            // Initialise each sex
            for(auto& sex : sexes) sex.initialise();
            
            // Take to virgin state
            virgin_state();
            // Set the virgin spawning biomass
            stock_recruit.relationship.s0 = biomass_spawning();
        }
        
        double recruits(void) const {
            return sexes[0].recruits() + sexes[1].recruits();
        }
        
        double biomass(void) const {
            return sexes[0].biomass() + sexes[1].biomass();
        }
        
        double biomass_spawning(void) const {
            return sexes[1].biomass_mature();
        }
        
        Fish& virgin_state(void){
            auto recruits_virgin = stock_recruit.recruits_virgin();
                        
            // The plus group must be intialised to zero
            // since it is += (i.e. 'plussed to') and thus existing values
            // matter
            sexes[male].set(0);
            sexes[female].set(0);
            
            //! @todo fix year<250
            for(int year=0;year<250;year++){
                ageing(recruits_virgin);
                mortality();
            }
            return *this;
        }
        
        Fish& ageing(const double& recruits){
            sexes[male].ageing(recruits*sex_ratio);
            sexes[female].ageing(recruits*(1-sex_ratio));
            return *this;
        }
        
        double recruits_calc(void){
            auto& self = *this;
            
            auto stock = self.biomass_spawning();
            auto recruits = self.stock_recruit(stock);
            return recruits;
        }
        
        Fish& mortality(void){
            auto& self = *this;
             
            self.sexes[male].mortality();
            self.sexes[female].mortality();
            
            return self;
        }
        
        //! @brief Modify the population due to fishing
        Fish& exploitation(const Date& date,const Fishing& fishing){
            auto& self = *this;
            
            for(auto& fleet : fishing.fleets){
                for(unsigned int sex=0;sex<Sexes;sex++){
                    self.sexes[sex].exploitation(date,fleet,fleet.sexes[sex]);
                }
            }

            return self;
        }
        
        Fish& start(const Date& date,const Fishing& fishing){
            auto& self = *this;
            
            auto stock_virgin = stock_recruit.stock_virgin();
            //! @todo fix year<100
            for(int year=0;year<100;year++){
                auto recruits = stock_recruit(stock_virgin);
                ageing(recruits);
                mortality();
            }
            
            return self;
        }
        
        Fish& update(const Date& date,const Fishing& fishing){
            auto& self = *this;

            self.ageing(self.recruits_calc());
            self.mortality();
            self.exploitation(date,fishing);
            
            return self;
        }
    
    };
    Fish fish;

    class Fishing {
    public:

        class Fleet {
        public:
            double catches;
            double biomass_vulnerable;
            double exploitation_rate;
            
            class Sex {
            public:
                
                typedef Math::Functions::Cached<DoubleNormalPlateau,Ages> Selectivities;
                Selectivities selectivities;
                
                void initialise(const typename Fish::Sex& sex){
                    selectivities.initialise_integrate(sex.sizes);
                }
            };
            std::array<Sex,2> sexes;
    
            void initialise(const Fish& fish){
                
                catches = 0;
                exploitation_rate = 0;
                
                for(int sex : {0,1}) {
                    auto& fish_sex = fish.sexes[sex];
                    sexes[sex].initialise(fish_sex);
                }
            }
            
            Fleet& start(const Date& date,const Fish& fish){
                auto& self = *this;
                
                return self;
            }
            
            double biomass_vulnerable_calc(const Date& date,const Fish& fish){
                auto& self = *this;
                 
                // Calculate vulnerable biomass
                double biomass = 0;
                for(auto& sex : {male,female}){
                    biomass += fish.sexes[sex].biomass_selected(self.sexes[sex].selectivities);
                }
                
                return biomass;
            }
            
            double exploitation_rate_calc(const Date& date,const Fish& fish){
                auto& self = *this;
                 
                //Calculate exploitation rate
                double rate = self.catches/self.biomass_vulnerable;
                //! @todo place limits on rate
                if(rate>1) rate = 1;
                
                return rate;
            }
            
            Fleet& update(const Date& date,const Fish& fish){
                auto& self = *this;
                
                self.biomass_vulnerable = self.biomass_vulnerable_calc(date,fish);
                self.exploitation_rate = self.exploitation_rate_calc(date,fish);
                
                return self;
            }
        };
        std::array<Fleet,Fleets> fleets;
        
        Fishing& initialise(const Fish& fish){
            auto& self = *this;
            
            for(auto& fleet : fleets) fleet.initialise(fish);
            
            return self;
        }
        
        Fishing& start(const Date& date,const Fish& fish){
            auto& self = *this;
            
            for(auto& fleet : fleets) fleet.start(date,fish);
            
            return self;
        }
        
        Fishing& update(const Date& date,const Fish& fish){
            auto& self = *this;
            
            for(auto& fleet : fleets) fleet.update(date,fish);
            
            return self;
        }
        
    };
    Fishing fishing;
    
    void initialise(void){
        fish.initialise(fishing);
        fishing.initialise(fish);
    }
    
    void start(const Date& date){
        fish.start(date,fishing);
        fishing.start(date,fish);
    }
    
    void update(const Date& date){
        fishing.update(date,fish);
        fish.update(date,fishing);
    }

};

} // namespace Matiri
} // namespace Models
} // namespace Fsl
