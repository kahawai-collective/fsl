#pragma once

namespace Fsl {
namespace Estimation {

class Fit {
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
class Fits : public Grid<Fit,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> {
public:

	typedef Grid<Fit,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> Base;

	double uncertainty;

	Fits(const double& uncertainty=0):
		uncertainty(uncertainty){
	}

	void read_observed(const std::string& path){
		Base::read(path,[](std::istream& stream, Fit& fit){
			stream>>fit.observed;
		});
	}

	void read_observed_uncertainty(const std::string& path){
		Base::read(path,[](std::istream& stream, Fit& fit){
			stream>>fit.observed>>fit.uncertainty;
		});
	}

	void write(const std::string& path){
		Base::write(path,{"observed","uncertainty","expected"},[](std::ostream& stream,const Fit& fit){
			stream<<fit.observed<<"\t"<<fit.uncertainty<<"\t"<<fit.expected;
		});
	}

	double likelihood(void){
		double likelihood = 0;
		for(auto fit : *this){
			if(std::isfinite(fit.observed) and std::isfinite(fit.expected)){
				double uncertainty = std::isfinite(fit.uncertainty)?(uncertainty+fit.uncertainty): uncertainty;
				Distribution distribution(fit.observed,fit.uncertainty);
				likelihood += log(distribution.pdf(fit.expected));
			}
		}
		return likelihood;
	}
};

template<
    class Derived,
    class Model
>
class DataGroup {
public:

    Derived& self(void) {
        return static_cast<Derived&>(*this);
    }

    const Derived& self(void) const {
        return static_cast<const Derived&>(*this);
    }

 };

} // namespace Estimation
} // namespace Fsl
