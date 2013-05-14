#pragma once

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
class Aged : public Array<Type,Ages> {
    
    FSL_PROPERTY(Aged,recruits,double)
    
public:
    static const int age_max = Ages-1;

    Sizes sizes;
    Weights weights;
    Maturities maturities;

    Recruitment recruitment;
    Mortality mortality;

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
    
    double biomass(void) const {
        auto& self = *this;
        double biomass = 0;
        for(int age=0;age<age_max;age++) biomass += self(age) * weights(age);
        return biomass;
    }
    
    double biomass_mature(void) const {
        auto& self = *this;
        double biomass = 0;
        for(int age=0;age<age_max;age++) biomass += self(age) * weights(age) * maturities(age);
        return biomass;
    }
    
    Aged& initialise(const double& number){
        auto& self = *this;
        for(int age=0;age<age_max;age++) self(age).initialise(number);
        sizes.initialise();
        weights.initialise();
        maturities.initialise();
        recruitment.initialise();
        mortality.initialise();
        return *this;
    }

    Aged& ageing(void){
        auto& self = *this;
        self(age_max).add(self(age_max-1));
        for(int age=age_max-1;age>0;age--) self(age) = self(age-1);
        self(0) = recruits();
        return self;
    }
};

template<
    class Type,
    class StockRecruit = Population::Recruitment::Stochastic<Population::Recruitment::BevertonHolt>
> 
class Sexed {

    FSL_PROPERTY(Sexed,split,double)

public:
    Type males;
    Type females;
    StockRecruit stock_recruit;
    
    double number(void) const {
        return males.number() + females.number();
    }
    
    Sexed& recruits(const double& value) {
        males.recruits(value * split());
        females.recruits(value * (1-split()));
        return *this;
    }
    
    double recruits(void) const {
        return males.recruits() + females.recruits();
    }
    
    double biomass(void) const {
        return males.biomass() + females.biomass();
    }
    
    // @{
    // @name Processes
    
    Sexed& initialise(const double& number){
        males.initialise(number);
        females.initialise(number);
        return *this;
    }
    
    Sexed& recruitment(void){
        auto stock = females.biomass_mature();
        auto recruit = stock_recruit(stock);
        recruits(recruit); 
        return *this;
    }

    Sexed& ageing(void){
        males.ageing();
        females.ageing();
        return *this;
    }
    
    template<
        class Fishing
    > Sexed& update(const Date& date, Fishing& fishing){
        recruitment();
        ageing();
        return *this;
    }
    
    // @}
};

}
}
}
