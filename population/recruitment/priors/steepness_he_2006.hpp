#pragma once

#include <boost/foreach.hpp>

#include <figo/probability/uniform.hpp>

namespace Figo {
namespace Population {
namespace Recruitment {
namespace Priors {
	
using Figo::Probability::Uniform;

/*!
A prior for steepness based on natural mortality and recruitment variability (He et al 2006)
*/
class SteepnessHe2006 {
public:

	struct Parameters {
		double Omega1;
		double Omega2;
		double Omega3;
	};
	Parameters parameters(const double& mort, const double& sigma) const{
		//Returns the parameter values for a prioir on steepness given natural mortality and sigma

		//Table 1 from He et al (2006).
		//Note that the value with "/*Iterpolated*/" next to it was missing in the original table and is interpolated from the omega1 values 'above' and 'below' it in the table
		//Note that the triptych marked with "/*As for <- */" was marked as "not fitted" in the table and I used the values for Sigma=1.6, M=0.6
		const double uniform = -1;
		double table[] = {
			uniform,uniform,uniform,uniform,uniform,1.00000e+0,1.00000e+0,1.00000e+0,1.00000e+0,1.00000e+0,
			uniform,uniform,uniform,uniform,uniform,6.69600e+2,4.43000e+2,3.06130e+2,2.82660e+2,2.78160e+2,
			uniform,uniform,uniform,uniform,uniform,9.28900e-1,8.04600e-1,6.66200e-1,5.01500e-1,3.72400e-1,
			uniform,9.99400e-1,1.00000e+0,1.00000e+0,1.00000e+0,1.00000e+0,9.99900e-1,9.99900e-1,9.99900e-1,9.99700e-1,
			uniform,1.39900e+2,2.48400e+2,2.23600e+2,1.96430e+2,1.99300e+2,1.82220e+2,1.57820e+2,1.48630e+2,1.45480e+2,
			uniform,9.80000e-1,7.97300e-1,5.58500e-1,3.73900e-1,2.14600e-1,8.61900e-2,4.45400e-2,2.26400e-2,1.15400e-2,
			uniform,1.00000e+0,1.00000e+0,1.00000e+0,9.99800e-1,9.99500e-1,9.99200e-1,9.98800e-1,9.98500e-1,9.97000e-1,/*Iterpolated*/
			uniform,1.49700e+2,1.33600e+2,1.16740e+2,1.09860e+2,1.05320e+2,8.95500e+1,7.87900e+1,6.92900e+1,6.14800e+1,
			uniform,7.55200e-1,3.89500e-1,1.79100e-1,8.06900e-2,3.80500e-2,1.54300e-2,7.60200e-3,5.07400e-3,3.86100e-3,
			1.00000e+0,1.00000e+0,1.00000e+0,9.99700e-1,9.99200e-1,9.99000e-1,9.98200e-1,9.97200e-1,9.95900e-1,9.94300e-1,
			8.30000e+1,9.31300e+1,8.42500e+1,7.62900e+1,6.87300e+1,6.55300e+1,4.88400e+1,4.05100e+1,3.40300e+1,2.91200e+1,
			9.25900e-1,4.31700e-1,1.37600e-1,4.54000e-2,2.02200e-2,1.19700e-2,5.79600e-3,3.74500e-3,3.04100e-3,2.72200e-3,
			1.00000e+0,1.00000e+0,9.99500e-1,9.98700e-1,9.98000e-1,9.97000e-1,9.94400e-1,9.90200e-1,9.83000e-1,9.70900e-1,
			7.53000e+1,6.44200e+1,5.75200e+1,4.87700e+1,4.14400e+1,3.55700e+1,2.71500e+1,2.15800e+1,1.77100e+1,1.49700e+1,
			9.52600e-1,1.92900e-1,4.46400e-2,1.70000e-1,9.24000e-3,6.19900e-3,3.90600e-3,3.07600e-3,2.68000e-3,2.40200e-3,
			1.00000e+0,9.99500e-1,9.98300e-1,9.96700e-1,9.94600e-1,9.91500e-1,9.80100e-1,9.54300e-1,9.01100e-1,8.08500e-1,
			6.93600e+1,4.81600e+1,3.87100e+1,3.09000e+1,2.53800e+1,2.12600e+1,1.57500e+1,1.26400e+1,1.10100e+1,1.03400e+1,
			5.92000e-1,7.18200e-2,1.84400e-2,9.03600e-3,5.81900e-3,4.42000e-3,3.13400e-3,2.24300e-3,1.32000e-3,5.88200e-4,
			1.00000e+0,9.98300e-1,9.95600e-1,9.91300e-1,9.83500e-1,9.69700e-1,9.02500e-1,7.42800e-1,4.97300e-1,2.42800e-1,
			4.77900e+1,3.50600e+1,2.58100e+1,1.98700e+1,1.59400e+1,1.33900e+1,1.07400e+1,1.02500e+1,1.08800e+1,1.26200e+1,
			2.02400e-1,2.50400e-2,9.16300e-3,5.64700e-3,4.01000e-3,3.08700e-3,1.37300e-3,3.18300e-4,3.68300e-5,1.60000e-6,
			9.99200e-1,9.95500e-1,9.88300e-1,9.71600e-1,9.33800e-1,8.58500e-1,5.46300e-1,2.02700e-1,2.79000e-2,2.79000e-2,/*As for <- */
			3.90900e+1,2.50300e+1,1.72200e+1,1.32200e+1,1.10800e+1,1.02500e+1,1.07500e+1,1.29000e+1,1.68500e+1,1.68500e+1,
			3.00100e-2,8.90000e-3,4.92300e-3,3.34000e-3,1.97100e-3,8.68600e-4,5.31500e-5,7.90000e-7,1.00000e-8,1.00000e-8
		};
		const double mortWidth = 0.05;
		const double sigmaWidth = 0.2;
		int mortClass = std::min(std::max(0,int((mort+mortWidth*0.5)/mortWidth)-1),9);
		int sigmaClass = std::min(std::max(0,int((sigma+sigmaWidth*0.5)/sigmaWidth)-1),7);
		int start = sigmaClass * 30 + mortClass;

		//Note that in the table caption the authors say that the parameters are in the order omega1,omega2,omega3
		//However, if one assumes this order you don't get sensible results. If one assumes the order omega1,omega3,omega2 do get sensible priors
		Parameters parameters;
		parameters.Omega1 = table[start];
		parameters.Omega3 = table[start+10];
		parameters.Omega2 = table[start+20];
		return parameters;
	}

	double relative(const double& steepness, const Parameters& pars) const {
		//Gives the *relative* probability of a given steepness, given the parameters of the prior
		const double o1 = pars.Omega1;
		const double o2 = pars.Omega2;
		const double o3 = pars.Omega3;
		return o1/(1+(o1-o2)/o2*std::exp(-o3*(steepness-0.2)/0.8));
	}
	
	std::vector<std::pair<double,double>> densities(const double& mortality,const double&sigma) const {
		Parameters pars = parameters(mortality,sigma);
		std::vector<std::pair<double,double>> densities;
		double sum = 0;
		double step = 0.01;
		for(double steepness=0.2;steepness<=1;steepness+=step){
			double probability = relative(steepness,pars);
			densities.push_back(std::pair<double,double>(steepness,probability));
			sum += probability * step;
		}
		//Normalise so that integral is 1
		typedef std::pair<double,double> Density;
		BOOST_FOREACH(Density& density,densities) density.second /= sum;
		return densities;
	}

	double random(const double& mortality,const double&sigma) const {
		static Uniform uniform(0,1);

		const double cumulativeProbsStep = 0.01;
		const int cumulativeProbsSteps = 81;
		double cumulativeProbs[81];
		Parameters pars = parameters(mortality,sigma);
		double cumul = 0;
		for(int step=0;step<cumulativeProbsSteps;step++){
			double steepness=0.2+step*cumulativeProbsStep;
			cumul += relative(steepness,pars);
			cumulativeProbs[step] = cumul;
		}
		
		//Normalise
		for(int step=0;step<cumulativeProbsSteps;step++) cumulativeProbs[step] /= cumul;

		double p = uniform.random();
		int step = 0;
		while(step<cumulativeProbsSteps){
			if(cumulativeProbs[step]>=p) break;
			step++;
		}
		if(step==0) return 0.2;
		if(step==cumulativeProbsSteps) return 1;
		
		//Intepolate
		double phi = cumulativeProbs[step];
		double steephi = 0.2+step*cumulativeProbsStep;
		double plo = cumulativeProbs[step-1];
		double steeplo = 0.2+(step-1)*cumulativeProbsStep;
		return steeplo+(steephi-steeplo)*(p-plo)/(phi-plo);
	}
};

}
}
}
}
