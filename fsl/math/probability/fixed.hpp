namespace Fsl {
namespace Math {
namespace Probability {

class Fixed {    
public:
    
    double value;

    Fixed(double val = NAN){
        value = val;
    }

    bool valid(void) const {
        return std::isfinite(value);
    }

    double minimum(void) const {
        return value;
    }

    double maximum(void) const {
        return value;
    }
    
    double mean(void) const {
        return value;
    }
    
    double sd(void) const {
        return 0;
    }

    double random(void) const {
        return value;
    };

    double likelihood(const double& x) const {
        return (x==value)?0:-INFINITY;
    }

    double pdf(const double& x) const {
        return (x==value)?1:0;
    }

    double quantile(const double& p) const {
        return value;
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(value,"value")
        ;
    }
};

}}}
