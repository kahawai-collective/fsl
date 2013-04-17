#pragma once

#include <fsl/math/functions/function.hpp>

#include <fsl/population/growth/von-bert.hpp>
#include <fsl/population/morphometry/morphometry.hpp>
#include <fsl/population/maturity/maturity.hpp>
#include <fsl/population/recruitment/beverton-holt.hpp>

#include <fsl/population/mortality/rate.hpp>

namespace Fsl {
namespace Population {
namespace Models {
	
class Unity {
public:	
};

class Numbers {
private:
    double value;

public:
    Numbers(const double& value_=0):
        value(value_){
    }
    
    operator double() const {
        return value;
    }
    
    Numbers& add(const Numbers& other){
        value += other;
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
public:
    static const int age_max = Ages-1;
	
	Sizes sizes;
	Weights weights;
	Maturities maturities;
	
	Recruitment recruitment;
	Mortality mortality;
	
	double recruits;

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

    Aged& ageing(void){
        auto& self = *this;
        self(age_max).add(self(age_max-1));
        for(int age=age_max-1;age>0;age--) self(age) = self(age-1);
        self(0) = recruits;
        return self;
    }
};

template<
    class Type,
	class Recruitment_ = Population::Recruitment::BevertonHolt
> 
class Sexed {
private:
	double proportion_;

public:
	Type males;
	Type females;
	
	typedef Recruitment_ Recruitment;
    Recruitment recruitment;
	
	double recruits;
		
	double proportion(void) const {
		return proportion_;
	}
	
	Sexed& proportion(const double& p) {
		proportion_ = p;
		return *this;
	}
    
    Sexed& proportion_set(const double& p){
        
    }

    Sexed& ageing(void){
		males.recruits = recruits * proportion;
		males.ageing();
		females.recruits = recruits * (1-proportion);
		females.ageing();
        return *this;
    }
};

}
}
}
