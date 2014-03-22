#pragma once

#include <boost/algorithm/string.hpp>

#include <fsl/math/probability/uniform.hpp>

namespace Fsl {
namespace Estimation {

namespace Links {

struct Identity {
    double to(const double& value) const {
        return value;
    }
    double from(const double& value) const {
        return value;
    }
};

struct Log {
    /**
     * Transform the parameter into a model variable
     * 
     * To avoid numerical overflow this function limits the
     * results of the exponential to approximately
     * 1e-20 to 1e20
     */
    double to(const double& value) const {
        static const double limit = 46;
        if(value<-limit) return std::exp(-limit);
        if(value>limit) return std::exp(limit);
        return std::exp(value);
        
    }

    /**
     * Transform the model variable into a parameter
     */
    double from(const double& value) const {
        return std::log(value);
    }
};

} // namespace Links

namespace Priors {

using Math::Probability::Uniform;

class Null : public Uniform {
public:
    Null(void): Uniform(-INFINITY,INFINITY){}
};

class Fixed {
public:
    double value;

    Fixed(void){}

    Fixed(const double& val): value(val) {}

    double minimum(void) const {
        return value;
    }

    double maximum(void) const {
        return value;
    }

    double mean(void) const {
        return value;
    }    

    double pdf(const double& x) const {
        return x==value?1:0;
    }
};

} // namespace Priors

template<
    class Prior = Priors::Null,
    class Link = Links::Identity
>
class Parameter {
public:
    std::string name;
    Prior prior;
    Link link;

    Parameter(void){
    }

    template<typename... Args>
    Parameter(const std::string name,Args... args):
        name(name),
        prior(args...){
    }

    double to(const double& value) const {
        if(value<prior.minimum()) return link.to(prior.minimum());
        if(value>prior.maximum()) return link.to(prior.maximum());
        return link.to(value);
    }

    double from(const double& value) const {
        return link.from(value);
    }
};

template<
    class Derived,
    class Model
>
class ParameterGroup {
public:

    Derived& self(void) {
        return static_cast<Derived&>(*this);
    }

    const Derived& self(void) const {
        return static_cast<const Derived&>(*this);
    }


    template<class Binder>
    void apply(Binder& binder, Model& model, uint time){
        self().bind(binder,model,time);
    }

    template<class Binder>
    void apply(Binder& binder, Model& model){
        for(uint time=self().first();time<=self().last();time++) apply(binder,model,time);
    }

    template<class Binder>
    void apply(Binder& binder){
        Model model;
        apply(binder,model);
    }


    struct Set {
        uint index = 0;
        std::vector<double> values; 

        Set& append(const double& value){
            values.push_back(value);
            return *this;
        }

        double next(void) {
            return values[index++];
        }
    };


    struct Setter {
        static const bool setter = true;
        Set& set_;

        Setter(Set& set):set_(set){}

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            variable = parameter.to(set_.next());
        }
    };

    void set(Model& model, uint time, Set& set){
        Setter setter(set);
        apply(setter,model,time);
    }


    struct PriorMeanGetter {
        static const bool setter = false;
        Set set;

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            set.append(parameter.prior.mean());
        }
    };

    Set means(void){
        PriorMeanGetter means;
        apply(means);
        return means.set;
    } 


    struct SetReader {
        static const bool setter = false;
        std::ifstream file;
        Set set;

        SetReader(const std::string& path){
            // Open the file and consume the header
            file.open(path);
            std::string header;
            std::getline(file,header);
        }

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            // Read data from file
            std::string line;
            std::getline(file,line);
            std::vector<std::string> items;
            boost::split(items,line,boost::is_any_of("\t "));
            // Check that they match
            if(items[0]!=parameter.name) throw std::runtime_error("Parameter name did not match expected: "+parameter.name+" : "+items[0]);
            if(items[1]!=label) throw std::runtime_error("Parameter label did not match expected: "+label+" : "+items[1]);
            // Get the value
            double value = boost::lexical_cast<double>(items[2]);
            set.append(value);
        }
    };

    Set read_set(const std::string& path){
        SetReader reader(path);
        apply(reader);
        return reader.set;
    }


    struct SetWriter {
        static const bool setter = false;
        std::ostream* stream;
        Set set;

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            (*stream)<<parameter.name<<"\t"<<label<<"\t"<<set.next()<<"\t"<<std::endl;
        }
    };

    void write_set(const std::string& filename,Set set){
        std::ofstream file(filename);
        file<<"name\tlabel\tvalue"<<std::endl;
        
        SetWriter writer;
        writer.stream = &file;
        writer.set = set;
        writer.set.index = 0;
        
        apply(writer);
    }


    struct PriorPdfWriter {
        static const bool setter = false;
        std::ostream* stream;

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            double lower = parameter.prior.minimum();
            double upper = parameter.prior.maximum();
            if(lower==upper){
                (*stream)<<parameter.name<<"\t"<<label<<"\t"<<upper<<"\t"<<parameter.prior.pdf(upper)<<"\t"<<std::endl;
            } else {
                double step = (upper-lower)/100;
                for(double value=lower;value<=upper;value+=step){
                    (*stream)<<parameter.name<<"\t"<<label<<"\t"<<value<<"\t"<<parameter.prior.pdf(value)<<"\t"<<std::endl;
                }
            }
        }
    };

    void write_pdfs(const std::string& filename){
        std::ofstream file(filename);
        file<<"name\tlabel\tvalue\tpdf"<<std::endl;
        
        PriorPdfWriter writer;
        writer.stream = &file;
        
        apply(writer);
    }

};

} // namespace Estimation
} // namespace Fsl
