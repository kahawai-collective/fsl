#pragma once

namespace Fsl {
namespace Monitoring {
namespace Composition {
	
template<
	unsigned int lowest,
	unsigned int highest,
	unsigned int step = 1,
	class Handle = int
>
class Sample {
public:
	static const unsigned int bins = (lowest-highest)/step+1;
	
	Sample(const Handle& handle, const double& size, const std::initializer_list<double>& proportions):
		handle(handle),
		size(size){
		//! @todo initialise proportions
	}
	
	Handle handle;
	double size;
	double proportions[bins];
};
	
template<
	unsigned int number,
	unsigned int lowest,
	unsigned int highest,
	unsigned int step = 1,
	class Handle = int
>
class Samples {
public:

	Samples(Sample<lowest,highest,step,Handle> samples[number]){
		//! @todo initialise samples
	}
};

}
}
}
