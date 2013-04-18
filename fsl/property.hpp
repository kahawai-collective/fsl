#pragma once

namespace Fsl {


};

#define FSL_PROPERTY_DEF(CLASS,NAME,TYPE) \
    template<typename Aide, typename... Others>                  \
    void NAME(Aide aide, Others... others) {                      \
        aide(*this,#NAME,&CLASS::NAME,&CLASS::NAME,others...);   \
    }                                                           \

#define FSL_PROPERTY(CLASS,NAME,TYPE)                           \
private:                                                        \
    TYPE NAME##_;                                               \
                                                                \
public:                                                         \
    FSL_PROPERTY_DEF(CLASS,NAME,TYPE)                                          \
                                                                \
    TYPE NAME(void) const {                                     \
        return NAME##_;                                         \
    }                                                           \
                                                                \
    CLASS& NAME(const TYPE& value) {                            \
        NAME##_ = value;                                        \
        return *this;                                           \
    }

