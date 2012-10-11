namespace Figo {
namespace Math {
namespace Probability {

class Fixed {
public:
    double value;
    
    Fixed(double value_ = 0){
        value = value_;
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
};

}}}
