#pragma once

#include "rate.hpp"

namespace Fsl {
namespace Population {
namespace Mortality {

class Instantaneous : public Rate {
public:

    Instantaneous(const double& value=0){
        instantaneous(value);
    }
};

}
}
}
