#pragma once

namespace Fsl {
namespace Estimation {

template<
    class Derived
>
class Variables : public Structure<Derived> {
public:

    typedef Structure<Derived> Base;

    void startup(void){
        read();
    }

    void shutdown(void){
        write();
    }

    void read(const std::string& path = "input/variables.json"){
        //Base::read(path);
    }

    void write(const std::string& path = "output/variables.json"){
        Base::write(path);
    }

};

} // namespace Estimation
} // namespace Fsl
