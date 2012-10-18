#pragma once

#include <figo/common.hpp>
#include <figo/array.hpp>

namespace Figo {
namespace Math {
namespace Functions {
    
/*!
 * A base class for all functions
 * 
 * This class is not expect to be used but it defines the interface
 * that all Functions are expected to have. Functions act as functors with
 * a "call operator" which takes a double and returns a double.
 */
class Function {
public:
    
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

    void reset(void){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(index);
        }
    }
    
    void reset(const double& base,const double& step){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(base+index*step);
        }
    }
    
    template<typename Callable>
    void reset(Callable callable){
        for(unsigned int index=0;index<ArrayType::size();index++) {
            ArrayType::operator()(index) = Function::operator()(callable(index));
        }
    }
    
    double operator()(const unsigned int& index) const {
        return ArrayType::operator()(index);
    }
};

}}}
