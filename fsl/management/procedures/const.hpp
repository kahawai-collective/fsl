#pragma once

#include <fsl/management/procedure.hpp>

namespace Fsl {
namespace Management {
namespace Procedures {

class CONST : public ControlProcedure<double> {
public:
    
    CONST(double* const control,const double& value=0):
        ControlProcedure(control,value){
    }
    
    std::string signature(void) const {
        return boost::str(boost::format("CONST\t%s\t0\t0\t0\t0\t0\t0\t0\t0\t0")
            %value
        );
    }
};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
