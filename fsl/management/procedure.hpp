#pragma once

#include <fstream>

namespace Fsl {
namespace Management {

class Procedure {
public:

    std::string tag = "";

    virtual std::string signature(void) const {
        return "";
    }

    virtual void reset(void) {
    }

    virtual void operate(uint time) {
    }

    virtual std::string record(void) const {
        return "";
    }

    /*
    void stream(uint time, const std::string& path = "") const {
        if (path.length() and not stream_) {
            stream_ = new std::ofstream(path);
        }
        if (not stream_) {
            throw Exception ("You need to call stream with a file path at least once");
        }
        stream_ << record(time);
    }
    */
    virtual ~Procedure (void) {
        delete stream_;
    }

private:

    std::ofstream* stream_ = nullptr;
};

template<
    typename Type = double
>
class ControlProcedure : public Procedure {
public:
    Type* control;
    Type value;

    ControlProcedure(Type* control,const Type& value = 0):
        control(control),
        value(value){
    }

    virtual void operate(uint time){
        *control = value;
    }
};

/**
 * Most management procedures are dynamic
 */
template<
    typename Type = double
>
class DynamicControlProcedure : public ControlProcedure<Type> {
public:
    Type starting;

    DynamicControlProcedure(Type* const control,const Type& starting = 0):
        ControlProcedure<Type>(control,starting),
        starting(starting){
    }
    
    virtual void reset(void){
        ControlProcedure<Type>::value = starting;
    }
};

} // namespace Management
} // namespace Fsl
