/*
C++ code skeleton for FiSL management procedure evaluation.
*/

#include <fisl/management/procedures/evaluation.hpp>
using Fisl::Management::Procedures::Evaluation;

#include <fisl/population/models/model.hpp>
using Fisl::Population::Models::Aged;
using Fisl::Population::Models::Sexed;

#include <fisl/math/functions/function.hpp>
using Fisl::Math::Functions::Cached;

#include <fisl/population/growth/vonbert.hpp>
using Fisl::Population::Growth::VonBert;
#include <fisl/population/morphometry/morphometry.hpp>
using Fisl::Population::Morphometry::Power;
#include <fisl/population/maturity/maturity.hpp>
using Fisl::Population::Maturity::Logistic;
#include <fisl/population/recruitment/bevertonholt.hpp>
using Fisl::Population::Recruitment::BevertonHolt;
#include <fisl/population/mortality/rate.hpp>
using Fisl::Population::Mortality::Rate;

#include <fisl/math/probability/distributions.hpp>
using namespace Fisl::Math::Probability;

#include <fisl/population/recruitment/priors/steepness_he_2006.hpp>

#include <fisl/estimation/mcmc/metropolis.hpp>
using Fisl::Estimation::Mcmc::Metropolis;

using namespace Fisl;

struct Fishery {
	Real catches;
	Logistic selectivities;
};

class MPE : public Evaluation<MPE> {
public:

	class Model {
	public:
		
		static const int ages = 80;
		Sexed<
			Aged<
				ages,
				Real,//Type
				Cached<VonBert,ages>, //Sizes
				Cached<Power,ages>, //Weights
				Cached<Logistic,ages>, //Maturities
				int, //Recruitment
				Rate //Mortality
			>,
			BevertonHolt //Recruitment
		> fish;
		
		class Fishing {
			
			Fishery line;
			Fishery trawl;
			
		} fishing;
		
	} model;
	
	void priors(void){
		auto& fish = model.fish;
		auto& males = fish.males;
		auto& females = fish.females;
		
		auto map = std::make_tuple(
			// Recruitment
			// Proportion of males
			&fish.proportion, 		Fixed(0.5),
			// Steepness
			// Cordue & Pomarède (2012) : base case 0.75; sensitivity 0.9
			&fish.recruitment.steepness, Population::Recruitment::Priors::SteepnessHe2006(),
			// Variation
			// Cordue & Pomarède (2012) : 0.6
			&fish.recruitment.sd,	Population::Recruitment::Priors::SigmaBentley2012(),
			
			// Instantaneous rate of natural mortality
			// Cordue & Pomerede (2012) : 0.08 for base case and did sensitivity analyses with 0.06 & 0.10
			//! @todo Consider using a 'flat-topped-triangular' distribution here
			&males.mortality, 		Uniform(0.06,0.1),
			&females.mortality, 	Uniform(0.06,0.1),
			
			// Size-at-age : von Bertalannfy function parameters
			// Horn et al (2010) estimates with 10% CV
			// Prior on cv of size-at-age same as used by Cordue & Pomarède (2012)
			&males.sizes.k, 		Normal(0.125,0.1,true),
			&males.sizes.linf, 		Normal(72.2,0.1,true),
			&males.sizes.t0, 		Fixed(-0.5),
			&males.sizes.cv,		Uniform(0.02,0.20),
			
			&females.sizes.k, 		Normal(0.071,0.1,true),
			&females.sizes.linf, 	Normal(92.5,0.1,true),
			&females.sizes.t0, 		Fixed(-0.5),
			&females.sizes.cv,		Uniform(0.02,0.20),
			
			// Weight-at-size : power function parameters
			&males.weights.a, 		Normal(0.00963,0.1,true),
			&males.weights.b, 		Normal(3.173,0.1,true),
			
			&females.weights.a, 	Normal(0.00963,0.1,true),
			&females.weights.b, 	Normal(3.173,0.1,true),
			
			// Maturity-at-age : logistic function parameters
			&males.maturities.inflection, 		Normal(15,0.1,true),
			&males.maturities.steepness, 		Normal(5,0.1,true),
			
			&females.maturities.inflection, 	Normal(17,0.1,true),
			&females.maturities.steepness, 		Normal(10,0.1,true),
			
			// Selectivity-at-size 
			// Uniformative priors based on length
			&line.selectivities.inflection,		Uniform(40,60),
			&line.selectivities.steepness,		Uniform(40,60),
			
			&trawl.selectivities.inflection,	Uniform(40,60),
			&trawl.selectivities.steepness,		Uniform(40,60)
		);
		
		struct catches {
			int year;
			int lo;
			int hi;
		};
		catches line[] = {
			{1936,0,150},
			{1937,0,150},
			{1938,0,150},
			{1939,0,150},
			{1940,0,112},
			{1941,0,100},
			{1942,0,100},
			{1943,0,100},
			{1944,0,100},
			{1945,0,100},
			{1946,0,138},
			{1947,0,150},
			{1948,0,162},
			{1949,0,189},
			{1950,0,177},
			{1951,0,147},
			{1952,0,142},
			{1953,0,141},
			{1954,0,137},
			{1955,0,132},
			{1956,0,138},
			{1957,0,138},
			{1958,0,149},
			{1959,0,137},
			{1960,0,124},
			{1961,0,121},
			{1962,0,118},
			{1963,0,119},
			{1964,0,133},
			{1965,0,128},
			{1966,0,123},
			{1967,0,129},
			{1968,0,113},
			{1969,0,111},
			{1970,0,140},
			{1971,0,138},
			{1972,0,118},
			{1973,0,126},
			{1974,0,137},
			{1975,111,252},
			{1976,618,767},
			{1977,821,1004},
			{1978,1,161},
			{1979,9,176},
			{1980,15,180},
			{1981,235,365},
			{1982,469,554},
			{1983,730,780},
			{1984,951,962},
			{1985,1013,1013},
			{1986,982,982},
			{1987,744,744},
			{1988,752,752},
			{1989,797,797},
			{1990,777,777},
			{1991,1192,1192},
			{1992,1414,1414},
			{1993,1573,1573},
			{1994,1459,1459},
			{1995,1382,1382},
			{1996,1503,1503},
			{1997,1765,1765},
			{1998,1728,1728},
			{1999,1871,1871},
			{2000,1712,1712},
			{2001,1638,1638},
			{2002,1443,1443},
			{2003,1671,1671},
			{2004,2133,2133},
			{2005,1900,1900},
			{2006,1765,1765},
			{2007,2001,2001},
			{2008,2000,2000},
			{2009,1746,1746},
			{2010,1759,1759},
			{2011,1700,1700},
			{2012,1700,1700}
		};
		
		catches trawl[] = {
			{1972,0,78},
			{1973,0,72},
			{1974,0,117},
			{1975,0,204},
			{1976,0,211},
			{1977,0,1505},
			{1978,0,0},
			{1979,0,0},
			{1980,0,0},
			{1981,0,0},
			{1982,0,0},
			{1983,0,0},
			{1984,324,324},
			{1985,372,372},
			{1986,605,605},
			{1987,667,667},
			{1988,522,522},
			{1989,623,623},
			{1990,763,763},
			{1991,577,577},
			{1992,549,549},
			{1993,733,733},
			{1994,860,860},
			{1995,904,904},
			{1996,811,811},
			{1997,1060,1060},
			{1998,779,779},
			{1999,904,904},
			{2000,1022,1022},
			{2001,1082,1082},
			{2002,1345,1345},
			{2003,1331,1331},
			{2004,957,957},
			{2005,1114,1114},
			{2006,710,710},
			{2007,424,424},
			{2008,500,500},
			{2009,300,300},
			{2010,300,300},
			{2011,300,300},
			{2012,300,300}
		};

	}

	/*!
	The conditioner conditions the model. That is, it produces a joint
	posterior parameter distribution upon which evaluations will be based.
	
	Various conditioner algorithms are available:
		Fisl::Estimation::Mcmc::Metropolis
	Derive from one of these and define the necessary methods
	*/
    class McMC : public Metropolis<McMC,1> {
    public:
        Real log_like(const Array<Real,1>& parameters){
            Real x = parameters(0);
    		return std::log(0.5-x);
	    }
    } mcmc;
	void posteriors(void){
		//mcmc.run(1000);
	}
	
    
} mpe;

int main(void){
    return mpe.run();
}

