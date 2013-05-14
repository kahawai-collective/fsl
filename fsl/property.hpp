#pragma once

namespace Fsl {

template<
    typename Class,
    typename Type
> class Property {
private:

    typedef Type (Class::* Getter)(void) const;
    typedef Class& (Class::* Setter)(const Type& value);

    Class* object_;
    std::string name_;
    Getter getter_;
    Setter setter_;

public:

    Property(Class& object,const char* name, Type (Class::* getter)(void) const, Class& (Class::* setter)(const Type& value)):
        object_(&object),
        name_(name),
        getter_(getter),
        setter_(setter){
    }
    
    std::string name(void) const {
        return name_;
    }
    
    Type get(void) const {
        return (object_->*getter_)();
    }
    
    Class& set(const Type& value) const {
        (object_->*setter_)(value);
        return *object_;
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
    (PROPERTY(_),OTHER)


