#pragma once

namespace Fsl {
namespace Math {
namespace Probability {

template<
    class Distribution,
    class Means,
    class Spreads,
    unsigned int Size
>
class Functional : public std::array<Distribution,Size> {
public:

    Means means;
    Spreads spreads;

    Functional& initialise(void){
        auto& self = *this;
        for(unsigned int index=0;index<Size;index++) {
            self[index] = Distribution(
                means(index),
                spreads(index)
            );
        }
        return self;
    }
};

} //namespace Fsl
} //namespace Math
} //namespace Probability