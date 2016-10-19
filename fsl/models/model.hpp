#pragma once

#include <stencila/structure.hpp>
using Stencila::Structure;

namespace Fsl {
namespace Models {

template<
	class Derived
>
class Model : public Structure<Derived> {
public:

	typedef Structure<Derived> Base;

	void startup(void){
		read();
	}

	void shutdown(void){
		write();
	}

	void read(const std::string& path = "input/model.json"){
		Base::read(path);
	}

	void write(const std::string& path = "output/model.json"){
		Base::write(path);
	}

	void update(void){
	}
};

}
}
