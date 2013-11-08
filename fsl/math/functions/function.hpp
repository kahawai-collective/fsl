#pragma once

#include <fsl/common.hpp>
#include <fsl/array.hpp>

namespace Fsl {
namespace Math {
namespace Functions {
    
/**
 * A base class for all functions
 * 
 * This class is not expected to be used but it defines the interface
 * that all Functions are expected to have. Functions act as functors with
 * a "call operator" which takes a double and returns a double.
 */
class Function {
public:
    
    /**
     * Get the value of the function
     * @param  x Point at which function is evaluated
     * @return   Value of function
     */
    double operator()(const double& x){
        return 0;
    }
};

/*!
 * @class Cached
 * @brief Precalculated function values for greater computational efficiency
 */
template<class Function, int Dimension>
class Cached : public Function, public Array<double,Dimension> {
public:

    typedef Array<double,Dimension> ArrayType;

    void initialise(void){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(index);
        }
    }
    
    void initialise(const double& base,const double& step){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(base+index*step);
        }
    }
    
    template<typename Callable>
    void initialise(Callable callable){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(callable(index));
        }
    }
    
    template<typename Distributions>
    void initialise_integrate(const Distributions& distributions){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            auto& distribution = distributions[index];
            auto integral = distribution.integrate(static_cast<Function&>(*this));
            ArrayType::operator()(index) = integral;
        }
    }
    
    double operator()(const double& value) const {
        return Function::operator()(value);
    }
    
    double operator[](const unsigned int& index) const {
        return ArrayType::operator()(index);
    }
    
}; // end class Cached 

} // end namespace Fsl
} // end namespace Math
} // end namespace Functions
