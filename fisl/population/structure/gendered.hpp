#pragma once

#include <fisl/array.hpp>

namespace Fisl {
namespace Population {
namespace Structure {

template<
    class Type
> 
class Gendered : public Array<Type,2> {
public:

    double proportion;
    
    Gendered& ageing(const double& recruits){
        (*this)(0).ageing(recruits * proportion);
        (*this)(1).ageing(recruits * (1-proportion));
        return *this;
    }
};

}
}
}
