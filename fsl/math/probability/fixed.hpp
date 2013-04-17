namespace Fsl {
namespace Math {
namespace Probability {

class Fixed {
private:
    double value_;
    
public:
    
    Fixed(double value = 0){
        value_ = value;
    }
    
    std::string repr(void) const {
        return str(boost::format("Fixed(%g)")%value());
    }
    
    double value(void) const {
        return value_;
    }
    
    double mean(void) const {
        return value_;
    }
    
    double sd(void) const {
        return 0;
    }

    double random(void) const {
        return value_;
    };
};

}}}
