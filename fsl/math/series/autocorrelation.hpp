#pragma once

#include <fsl/property.hpp>

namespace Fsl {
namespace Math {
namespace Series {

class Autocorrelation {
private:

    bool started_;
    double last_;

public:

    FSL_PROPERTY(Autocorrelation,coefficient,double)
    
    Autocorrelation(void):
        started_(false),
        last_(0),
        coefficient_(0){
    }

    double operator()(const double& deviation) {
        if(started_) {
            last_ = coefficient_*last_ + std::sqrt(1-std::pow(coefficient_,2))*deviation;
        }
        else {
            last_ = deviation;
            started_ = true;
        }
        return last_;
    }
};

}
}
}

