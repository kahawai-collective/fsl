#pragma once

#include <vector>
#include <iostream>
#include <cassert>

#include <fsl/common.hpp>
#include <fsl/math/functions/function.hpp>

namespace Fsl {
namespace Math {
namespace Functions {

class PiecewiseSpline {
public:

    std::vector<double> xs;
    std::vector<double> ys;

    PiecewiseSpline(const std::vector<double>& xs, const std::vector<double>& ys):
        xs(xs),ys(ys){}

    double interpolate(const double& x) const {
        for(uint index=0;index<xs.size()-1;index++){
            if(xs[index]<=x and x<xs[index+1]){
                return ys[index] + (x-xs[index]) * (ys[index+1]-ys[index])/(xs[index+1]-xs[index]);
            }
        }
        throw std::runtime_error("Attempting to interpolate outside of range of x values");
    }

}; // class PiecewiseSpline

} // namespace Functions
} // namespace Math
} // namespace Fsl
