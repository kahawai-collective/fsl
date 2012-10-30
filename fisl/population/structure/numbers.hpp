#pragma once

namespace Fisl {
namespace Population {
namespace Structure {
    
class Numbers {
private:
    double value;

public:
    Numbers(const double& value_=0):
        value(value_){
    }
    
    operator double() const {
        return value;
    }
    
    Numbers& add(const Numbers& other){
        value += other;
        return *this;
    }
};

}
}
}
