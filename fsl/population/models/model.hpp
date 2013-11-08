#pragma once

#include <array>

#include <fsl/date.hpp>

#include <fsl/math/functions/function.hpp>

#include <fsl/population/mortality/rate.hpp>
#include <fsl/population/growth/von-bert.hpp>
#include <fsl/population/morphometry/morphometry.hpp>
#include <fsl/population/maturity/maturity.hpp>
#include <fsl/population/recruitment/beverton-holt.hpp>
#include <fsl/population/recruitment/stochastic.hpp>


namespace Fsl {
namespace Population {
namespace Models {

class Unity {
public:	
};

class Numbers {
private:
    double value_;

public:
    Numbers(const double& value=0):
        value_(value){
    }
    
    operator double() const {
        return value_;
    }
    
    Numbers& add(const Numbers& other){
        value_ += other;
        return *this;
    }
    
    Numbers& operator*=(const Numbers& other){
        value_ *= other;
        return *this;
    }
    
    Numbers& initialise(const double& number){
        value_ = number;
        return *this;
    }
};

template<
    int Ages,
    class Type = Numbers,
    class Sizes = Math::Functions::Cached<Population::Growth::VonBert,Ages>,
    class Weights = Math::Functions::Cached<Population::Morphometry::Power,Ages>,
    class Maturities = Math::Functions::Cached<Population::Maturity::Threshold,Ages>,
    class Recruitment = Population::Recruitment::BevertonHolt,
    class Mortality = Population::Mortality::Rate
> 
class Aged : public std::array<Type,Ages> {
public:
    static const int age_max = Ages-1;

    Sizes sizes;
    Weights weights;
    Maturities maturities;
    Mortality mortalities;
    
    Recruitment recruitment;

    Aged(void){
    }

    Aged(const std::initializer_list<Type>& values){
        int age = 0;
        auto i = values.begin();
        while(age<Ages and i!=values.end()){
            (*this)(age) = *i;
            age++;
            i++;
        }
    }
    
    double recruits(void) const {
        auto& self = *this;

        return self[0];
    }
    
    double biomass(void) const {
        auto& self = *this;
        
        double biomass = 0;
        for(int age=0;age<age_max;age++) biomass += self[age] * weights[age];
        
        return biomass;
    }
    
    double biomass_mature(void) const {
        auto& self = *this;
        
        double biomass = 0;
        for(int age=0;age<age_max;age++) biomass += self[age] * weights[age] * maturities[age];
        
        return biomass;
    }
    
    template<
        class Selectivity
    >
    double biomass_vulnerable(const Selectivity& selectivity) const {
        auto& self = *this;
        
        double biomass = 0;
        for(int age=0;age<age_max;age++) biomass += self[age] * weights[age] * selectivity[age];
        
        return biomass;
    }
    
    Aged& initialise(void){
        auto& self = *this;
        
        sizes.initialise();
        weights.initialise(sizes);
        maturities.initialise();
        mortalities.initialise();
        
        recruitment.initialise();
        
        return self;
    }

    Aged& ageing(const double& recruits){
        auto& self = *this;
        
        self[age_max].add(self[age_max-1]);
        for(int age=age_max-1;age>0;age--) self[age] = self[age-1];
        self[0] = recruits;
        
        return self;
    }
    
    Aged& mortality(void){
        auto& self = *this;
        
        for(int age=0;age<age_max;age++) self[age] *= self.mortalities[age].survival();
            
        return self;
    }
};

template<
    class Type,
    class StockRecruit = Population::Recruitment::Stochastic<Population::Recruitment::BevertonHolt>
> 
class Sexed {

    double split;

public:
    Type males;
    Type females;
    StockRecruit stock_recruit;
    
    double number(void) const {
        return males.number() + females.number();
    }
    
    double recruits(void) const {
        return males.recruits() + females.recruits();
    }
    
    double biomass(void) const {
        return males.biomass() + females.biomass();
    }
    
    double biomass_spawning(void) const {
        return females.biomass_mature();
    }
    
    template<
        class Selectivity
    >
    double biomass_vulnerable(const Selectivity& selectivity) const {
         return males.biomass_vulnerable(selectivity) + females.biomass_vulnerable(selectivity);
    }
    
    //! @{
    //! @name Processes
    
    Sexed& initialise(void){
        males.initialise();
        females.initialise();
        
        return *this;
    }
    
    Sexed& virgin_state(void){
        auto recruits_virgin = stock_recruit.recruits_virgin();
        for(int year=0;year<500;year++){
            ageing(recruits_virgin);
            mortality();
        }
        return *this;
    }
    
    Sexed& initial_state(void){
        auto stock_virgin = stock_recruit.stock_virgin();
        for(int year=0;year<300;year++){
            auto recruits = stock_recruit(stock_virgin);
            ageing(recruits);
            mortality();
        }
        return *this;
    }
    
    Sexed& ageing(const double& recruits){
        males.ageing(recruits*split());
        females.ageing(recruits*(1-split()));

        return *this;
    }
    
    Sexed& mortality(void){
        males.mortality();
        females.mortality();

        return *this;
    }
    
    Sexed& update(const Date& date){
        auto stock = biomass_spawning();
        auto recruits = stock_recruit(stock);
        ageing(recruits);
        mortality();
        return *this;
    }
    
    //! @}
    
    //! @{
    //! @name Output methods
    
    std::string json(void){
        std::stringstream json;
        
        json<<"{";
        json<<"\"males\":"<<males.json();
        json<<",";
        json<<"\"females\":"<<females.json();
        json<<"}";
        
        return json.str();
    }
    
    //! @}
};

}
}
}
