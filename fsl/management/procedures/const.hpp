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
        return boost::str(boost::format("CONST(%s)")
            %value
        );
    }
};

} // namespace Procedures
} // namespace Management
} // namespace Fsl
