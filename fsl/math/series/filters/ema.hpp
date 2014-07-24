#pragma once

#include <cmath>

namespace Fsl {
namespace Math {
namespace Series {
namespace Filters {

class Ema {
public:
    double coefficient;
    double value;
    
    Ema(const double& coefficient):
        coefficient(coefficient){
        reset();
    }

    void operator=(const double& other) {
        value = other;
    }

    operator double(void) const {
        return value;
    }

    Ema& reset(void){
        value = NAN;
        return *this;
    }

    double update(const double& current){
        if(std::isnan(value)) value = current;
        else value = coefficient*current + (1-coefficient)*value;
        return value;
    }
};

}
}
}
}
