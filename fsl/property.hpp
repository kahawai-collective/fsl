#pragma once

namespace Fsl {

template<
    typename Class,
    typename Type
> class Property {
private:

    std::string name_;

public:
    Property(Class& object,const char* name, Type (Class::* getter)(void) const, Class& (Class::* setter)(const Type& value)){
        name_ = name;
    }
    
    std::string name(void) const {
        return name_;
    }
};

class PropertySymbol {};
static const PropertySymbol _;

};

#define FSL_PROPERTY_DEF(CLASS,NAME,TYPE) \
    template<typename Aide, typename... Others>                 \
    void NAME(Aide aide, Others... others) {                    \
        aide(*this,#NAME,&CLASS::NAME,&CLASS::NAME,others...);  \
    }                                                           \
                                                                \
    Property<CLASS,TYPE> NAME(const PropertySymbol& property) {       \
        return Property<CLASS,TYPE>(*this,#NAME,&CLASS::NAME,&CLASS::NAME);  \
    }                                                           \

#define FSL_PROPERTY(CLASS,NAME,TYPE)                           \
private:                                                        \
    TYPE NAME##_;                                               \
                                                                \
public:                                                         \
    FSL_PROPERTY_DEF(CLASS,NAME,TYPE)                           \
                                                                \
    TYPE NAME(void) const {                                     \
        return NAME##_;                                         \
    }                                                           \
                                                                \
    CLASS& NAME(const TYPE& value) {                            \
        NAME##_ = value;                                        \
        return *this;                                           \
    }

#define __(PROPERTY,OTHER) \
    (#PROPERTY,PROPERTY(_),OTHER)


