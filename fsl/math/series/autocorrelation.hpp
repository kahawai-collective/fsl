#pragma once

namespace Fsl {
namespace Math {
namespace Series {

class Autocorrelation {
private:

    bool started_;
    double last_;

public:

    double coefficient;
    
    Autocorrelation(void):
        started_(false),
        last_(0),
        coefficient(0){
    }

    double operator()(const double& deviation) {
        if(started_) {
            last_ = coefficient*last_ + std::sqrt(1-std::pow(coefficient,2))*deviation;
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

