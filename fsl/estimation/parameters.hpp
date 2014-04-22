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

        std::vector<std::string> ids;

        Set& append(const double& value,const std::string& name,const std::string& label){
            values.push_back(value);
            std::string id = name;
            if(label.length()>0) id += "." + label;
            ids.push_back(id);
            return *this;
        }

        void begin(void) {
            index = 0;
        }

        double next(void) {
            return values[index++];
        }

        uint find(const std::string& id){
            auto found = std::find(ids.begin(),ids.end(),id);
            if(found==ids.end()) throw std::runtime_error("No parameter found with id:"+id);
            return found-ids.begin();
        }

        double get(const std::string& id){
            return values[find(id)];
        }

        void set(const std::string& id,double value){
            values[find(id)] = value;
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
            set.append(parameter.prior.mean(),parameter.name,label);
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
            if(items[1]!=label) throw std::runtime_error("Parameter label did not match expected for parameter: "+parameter.name+" : "+label+" : "+items[1]);
            // Get the value and append it
            double value = boost::lexical_cast<double>(items[2]);
            set.append(value,parameter.name,label);
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


    struct Profiler {
        static const bool setter = false;
        std::string name;
        double min;
        double max;

        Profiler(const std::string& name):
            name(name){
        }

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            if(parameter.name==name){
                min = parameter.prior.minimum();
                max = parameter.prior.maximum();
            }
        }
    };

    template<
        class Data
    >
    void profile(const std::string& parameter, const Set& parameters, const Data& data, const std::string& filename){
        // Open the file
        std::ofstream file(filename);
        // Define a profiler which will change the values of the parameter of interest
        Profiler profiler(parameter);
        apply(profiler);
        // Create a copy of the parameter set
        Set modified = parameters;

        double jump = (profiler.max-profiler.min)/30;
        for(double value=profiler.min;value<=profiler.max;value+=jump){
            file<<value;
            modified.set(parameter,value);
            modified.begin();

            Model model;
            Data data_ = data;
            for(uint time=self().first();time<=self().last();time++){
                // Apply the parameter set to the model
                set(model,time,modified);
                model.update(time);
                data_.get(model,time);
            }

            auto likes = data_.likelihoods();
            for(auto like : likes){
                file<<"\t"<<like;
            }
            file<<std::endl;
        }
    }
};

} // namespace Estimation
} // namespace Fsl
