#pragma once

#include <fisl/array.hpp>

namespace Fisl {
namespace Population {
namespace Structure {
    
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
    class Type,
    int Ages
> 
class Aged : public Array<Type,Ages> {
public:
    static const int age_max = Ages-1;

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

    Aged& ageing(const Type& recruits){
        auto& self = *this;
        self(age_max).add(self(age_max-1));
        for(int age=age_max-1;age>0;age--) self(age) = self(age-1);
        self(0) = recruits;
        return self;
    }
};

}
}
}
