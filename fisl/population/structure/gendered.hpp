#pragma once

#include <fisl/array.hpp>

namespace Fisl {
namespace Population {
namespace Structure {

enum Sex {male=0,female=1};
	
template<
    class Type
> 
class Gendered : public Array<Type,2> {
public:
	
    double proportion;
	double recruits;
	
    Gendered& ageing(void){
		for(unsigned int g=0;g<=1;g++){
			Type& gender = (*this)(g);
			gender.recruits = recruits * proportion;
			gender.ageing();
		}
        return *this;
    }
};

}
}
}
