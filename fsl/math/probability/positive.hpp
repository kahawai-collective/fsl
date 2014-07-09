namespace Fsl {
namespace Math {
namespace Probability {

class Positive {    
public:
    
    double value;

    Positive(double value_ = NAN):
        value(value_){
    }

    bool valid(void) const {
        return std::isfinite(value) and value>0;
    }

    double minimum(void) const {
        return 0;
    }

    double maximum(void) const {
        return INFINITY;
    }
    
    double mean(void) const {
        return INFINITY;
    }
    
    double sd(void) const {
        return INFINITY;
    }

    double random(void) const {
        return NAN;
    };

    double likelihood(const double& x) const {
        return NAN;
    }

    double pdf(const double& x) const {
        return 0;
    }

    double quantile(const double& p) const {
        return NAN;
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(value,"value")
        ;
    }
};

}}}
