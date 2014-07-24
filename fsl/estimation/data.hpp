#pragma once

#include <fsl/estimation/variables.hpp>

namespace Fsl {
namespace Estimation {

template<class Distribution> using Datum = Variable<Distribution>;
template<class Distribution, class... Indices> using Datums = Variables<Distribution,Indices...>;

template<
    class Derived
>
class DataSet : public Set<Derived> {
public:

    DataSet(const std::string& path="data"):
        Set<Derived>(path){
    }
};

} // namespace Estimation
} // namespace Fsl

#if 0
#include <fsl/estimation/variables.hpp>

namespace Fsl {
namespace Estimation {

class DataFit {
public:
	double observed = NAN;
	double uncertainty = NAN;
	double expected = NAN;
};

template<
	class Distribution,
	class D1 = Singular1,
	class D2 = Singular2,
	class D3 = Singular3,
	class D4 = Singular4,
	class D5 = Singular5,
	class D6 = Singular6,
	class D7 = Singular7,
	class D8 = Singular8,
	class D9 = Singular9,
	class D10 = Singular10
>
class DataFits : public Array<DataFit,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> {
public:

	typedef Array<DataFit,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> Base;

	double uncertainty;

	DataFits(const double& uncertainty=0):
		uncertainty(uncertainty){
	}

	void read_observed(const std::string& path){
		Base::read(path,[](std::istream& stream, DataFit& fit){
			stream>>fit.observed;
		});
	}

	void read_observed_uncertainty(const std::string& path){
		Base::read(path,[](std::istream& stream, DataFit& fit){
			stream>>fit.observed>>fit.uncertainty;
		});
	}

	void write(const std::string& path){
		Base::write(path,{"observed","uncertainty","expected"},[](std::ostream& stream,const DataFit& fit){
			stream<<fit.observed<<"\t"<<fit.uncertainty<<"\t"<<fit.expected;
		});
	}

	double likelihood(void){
		double likelihood = 0;
		for(auto fit : *this){
			if(std::isfinite(fit.observed) and std::isfinite(fit.expected)){
				double error = std::isfinite(fit.uncertainty)?(uncertainty+fit.uncertainty):uncertainty;
				Distribution distribution(fit.observed,error);
				likelihood += log(distribution.pdf(fit.expected));
			}
		}
		return likelihood;
	}
};

} // namespace Estimation
} // namespace Fsl
#endif
