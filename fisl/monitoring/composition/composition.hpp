#pragma once

namespace Fisl {
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
	
	Sample(const Handle& handle, const Real& size, const std::initializer_list<Real>& proportions):
		handle(handle),
		size(size){
		//! @todo initialise proportions
	}
	
	Handle handle;
	Real size;
	Real proportions[bins];
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
