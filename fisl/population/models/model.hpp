#pragma once

#include <fisl/math/functions/function.hpp>

#include <fisl/population/growth/vonbert.hpp>
#include <fisl/population/maturity/maturity.hpp>
#include <fisl/population/recruitment/bevertonholt.hpp>

#include <fisl/population/mortality/rate.hpp>

namespace Fisl {
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
	class Weights = Unity,
	class Maturities = Math::Functions::Cached<Population::Maturity::Threshold,Ages>,
	class Mortality = Population::Mortality::Rate
> 
class Aged : public Array<Type,Ages> {
public:
    static const int age_max = Ages-1;
	
	Sizes sizes;
	Weights weights;
	Maturities maturities;
	
	Real recruits;
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

    Aged& ageing(void){
        auto& self = *this;
        self(age_max).add(self(age_max-1));
        for(int age=age_max-1;age>0;age--) self(age) = self(age-1);
        self(0) = recruits;
        return self;
    }
};

template<
    class Type
> 
class Gendered {
public:
	
	Real recruits;
	Real proportion;
	
    Type male;
	Type female;
	
    Gendered& ageing(void){
		male.recruits = recruits * proportion;
		male.ageing();
		female.recruits = recruits * (1-proportion);
		female.ageing();
        return *this;
    }
};

}
}
}
