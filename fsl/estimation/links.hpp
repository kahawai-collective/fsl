#pragma once

namespace Fsl {
namespace Estimation {
namespace Links {

class Identity {
public:
    double to(const double& value) const {
        return value;
    }
    double from(const double& value) const {
        return value;
    }
};

class Log {
public:
    /**
     * Transform the parameter into a model variable
     * 
     * To avoid numerical overflow this function limits the
     * results of the exponential to approximately
     * 1e-20 to 1e20
     */
    double to(const double& value) const {
        static const double limit = 46;
        if(value<-limit) return std::exp(-limit);
        if(value>limit) return std::exp(limit);
        return std::exp(value);
        
    }

    /**
     * Transform the model variable into a parameter
     */
    double from(const double& value) const {
        return std::log(value);
    }
};

} //namespace Links
} //namespace Estimation
} //namespace Fsl
