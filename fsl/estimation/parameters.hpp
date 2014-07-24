#pragma once

#include <fsl/estimation/variables.hpp>

namespace Fsl {
namespace Estimation {

template<class Distribution> using Parameter = Variable<Distribution>;
template<class Distribution, class... Indices> using Parameters = Variables<Distribution,Indices...>;

template<
    class Derived
>
class ParameterSet : public Set<Derived> {
public:

    ParameterSet(const std::string& path="parameters"):
        Set<Derived>(path){
    }
};

} // namespace Estimation
} // namespace Fsl


#if 0
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <fsl/estimation/variables.hpp>
#include <fsl/math/probability/uniform.hpp>

namespace Fsl {
namespace Estimation {

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
    std::string notes;
    Prior prior;
    Link link;

    Parameter(void){
    }

    Parameter(const double& p1):
        prior(p1){
    }

    Parameter(const double& p1,const double& p2):
        prior(p1,p2){
    }

    template<typename... Args>
    Parameter(const std::string& name,Args... args):
        name(name),
        prior(args...){
    }

    template<typename... Args>
    Parameter(const std::string& name,const std::string& notes,Args... args):
        name(name),
        notes(notes),
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

/**
 * A set of parameters
 */
class ParameterSample : public std::vector<double> {
private:
    uint index_ = 0;
    std::vector<std::string> ids_;

public:
    ParameterSample(void){}

    template<typename Type>
    ParameterSample(Type data):std::vector<double>(data){};

    ParameterSample& append(const double& value){
        push_back(value);
        return *this;
    }

    ParameterSample& append(const double& value,const std::string& name,const std::string& label=""){
        push_back(value);
        std::string id = name;
        if(label.length()>0) id += "." + label;
        ids_.push_back(id);
        return *this;
    }

    //void begin(void) {
    //   index_ = 0;
    //}

    double next(void) {
        return (*this)[index_++];
    }

    uint find(const std::string& id){
        auto found = std::find(ids_.begin(),ids_.end(),id);
        if(found==ids_.end()) throw std::runtime_error("No parameter found with id:"+id);
        return found-ids_.begin();
    }

    double get(const std::string& id){
        return (*this)[find(id)];
    }

    void set(const std::string& id,double value){
        (*this)[find(id)] = value;
    }
};

class ParameterSamples : public std::vector<ParameterSample> {
private:

    std::vector<std::string> ids_;
    ParameterSample sample_;

public:

    ParameterSamples(void){}
    
    template<typename Type>
    ParameterSamples(Type data):std::vector<ParameterSample>(data){};

    ParameterSample random(void) {
        uint row = std::rand()%size();
        sample_ = operator[](row);
        return sample_;
    }

    double get(const std::string& id){
        uint column = std::find(ids_.begin(),ids_.end(),id)-ids_.begin();
        return sample_[column];
    }

    /**
     * Read a file of tab separated values
     * 
     * @param  filename Filename to read
     * @param  header   Does the file have a header
     * @return          [description]
     */
    ParameterSamples& read(const std::string& filename,bool header=true){
        std::ifstream file(filename);
        std::string line;

        boost::regex rgx("\\s+");
        boost::sregex_token_iterator end;
                
        if(header){
            std::getline(file, line);
            boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
            for( ; iter != end; ++iter) ids_.push_back(*iter);
        }

        while(true){
            std::getline(file, line);
            if(not file.good()) break;
            boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
            ParameterSample sample;
            for( ; iter != end; ++iter) sample.push_back(boost::lexical_cast<float>(*iter));
            push_back(sample);
        }

        return *this;
    }

    void read_ss3(const std::string& filename){
        // Read in posterior
        std::ifstream file(filename);
        std::string line;

        // Get header
        std::getline(file, line);
        boost::regex rgx("\\s+");
        boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
        boost::sregex_token_iterator end;
        for( ; iter != end; ++iter) ids_.push_back(*iter);

        while(true){
            std::getline(file, line);
            if(not file.good()) break;
            boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
            ParameterSample sample;
            for( ; iter != end; ++iter) sample.push_back(boost::lexical_cast<float>(*iter));
            push_back(sample);
        }
    }

    void write(const std::string& filename){
        std::ofstream file(filename);
        if(ids_.size()>0){
            for(auto label : ids_) file<<label<<"\t";
            file<<std::endl;
        }
        for(auto row : *this){
            for(auto value : row) file<<value<<"\t";
            file<<std::endl;
        }
    }

};

template<
    class Derived,
    class Model
>
class ParameterSet {
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


    struct Setter {
        static const bool setter = true;
        ParameterSample& sample_;

        Setter(ParameterSample& sample):sample_(sample){}

        void operator()(double& variable, uint index){
            variable = sample_[index];
        }

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            variable = parameter.to(sample_.next());
        }
    };

    void set(Model& model, uint time, ParameterSample& sample){
        Setter setter(sample);
        apply(setter,model,time);
    }


    struct PriorMeanGetter {
        static const bool setter = false;
        ParameterSample sample;

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            sample.append(parameter.prior.mean(),parameter.name,label);
        }
    };

    ParameterSample means(void){
        PriorMeanGetter means;
        apply(means);
        return means.sample;
    } 


    struct SetReader {
        static const bool setter = false;
        std::ifstream file;
        ParameterSample sample;

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
            sample.append(value,parameter.name,label);
        }
    };

    ParameterSample read_set(const std::string& path){
        SetReader reader(path);
        apply(reader);
        return reader.sample;
    }


    struct SetWriter {
        static const bool setter = false;
        std::ostream* stream;
        ParameterSample sample;

        template<class Parameter>
        void operator()(const Parameter& parameter,double& variable,const std::string& label=""){
            (*stream)<<parameter.name<<"\t"<<label<<"\t"<<sample.next()<<"\t"<<std::endl;
        }
    };

    void write_set(const std::string& filename,ParameterSample sample){
        std::ofstream file(filename);
        file<<"name\tlabel\tvalue"<<std::endl;
        
        SetWriter writer;
        writer.stream = &file;
        writer.sample = sample;
        writer.sample.index = 0;
        
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
    void profile(const std::string& parameter, const ParameterSample& parameters, const Data& data, const std::string& filename){
        // Open the file
        std::ofstream file(filename);
        // Define a profiler which will change the values of the parameter of interest
        Profiler profiler(parameter);
        apply(profiler);
        // Create a copy of the parameter set
        ParameterSample modified = parameters;

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
    /*

    struct Counter {
        uint count = 0;

        template<class Parameter>
        void operator()(const std::string& name, double& value, Parameter& parameter){
            count++;
        }
    };

    uint count(void) const {
        Model model;
        Counter counter;
        bind(model,counter);
        return counter.count;
    }

    struct Headerer {
        std::string header;

        template<class Parameter>
        void operator()(const std::string& name, double& target, Parameter& parameter){
            if(header.length()>0) header += "\t";
            header += name;
        }
    };


    struct Getter {
        uint index = 0;
        std::vector<double> parameters;

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            parameters.push_back(parameter.from(variable));
        }
    };

    std::vector<double> get(Model& model){
        Getter getter;
        bind(model,getter);
        return getter.parameters;
    }


    struct Setter {
        uint index = 0;
        std::vector<double> parameters;

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            variable = parameter.to(parameters[index++]);
        }
    };

    void set(Model& model, const std::vector<double>& parameters){
        Setter setter;
        setter.parameters = parameters;
        bind(model,setter);
    }


    struct Mean {
        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            variable = parameter.to(parameter.prior.mean());
        }
    };

    void means(Model& model){
        Mean mean;
        bind(model,mean);
    }


    struct PriorParameterSampler {
        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            variable = parameter.to(parameter.prior.random());
        }
    };

    std::vector<double> prior_sample(void){
        Model model;
        PriorParameterSampler sampler;
        bind(model,sampler);
        return get(model);
    }

    struct Validator {
        uint index = 0;
        std::vector<double> parameters;
        bool ok = true;

        Validator(const std::vector<double>& parameters):
            parameters(parameters){
        }

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            double value = parameters[index++];
            if(value<parameter.prior.minimum() or value>parameter.prior.maximum()){
                ok = false;
            }
        }
    };

    bool valid(const std::vector<double>& parameters){
        Model model;
        Validator validator(parameters);
        bind(model,validator);
        return validator.ok;
    }

    struct Restricter {
        uint index = 0;
        std::vector<double> ins;
        std::vector<double> outs;

        Restricter(const std::vector<double>& parameters):
            ins(parameters),
            outs(parameters.size()){
        }

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            double out;
            double in = ins[index];
            if(in<parameter.prior.minimum() or in>parameter.prior.maximum()) out = parameter.prior.random(); 
            else out = in;
            outs[index] = out;
            index++;
        }
    };

    std::vector<double> restrict(const std::vector<double>& parameters){
        Model model;
        Restricter restricter(parameters);
        bind(model,restricter);
        return restricter.outs;
    }

    struct Probability {
        double probability = 0;

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            probability += std::log(parameter.prior.pdf(parameter.from(variable)));
        }
    };

    double prior_probability(const std::vector<double>& parameters){
        Model model;
        Setter setter;
        setter.parameters = parameters;
        bind(model,setter);
        Probability probability;
        bind(model,probability);
        return probability.probability;
    }

    std::string header(){
        Model model;
        Headerer headerer;
        bind(model,headerer);
        return headerer.header;
    }

    void parameters(std::ostream& stream, const std::vector<double>& parameters){
        uint size = parameters.size();
        for(uint index=0;index<size;index++){
            stream<<parameters[index];
            if(index<size-1) stream<<"\t";
        }
    }

    struct ValueWriter {
        std::ostream* stream;
        bool started = false;

        template<class Distribution>
        void operator()(const std::string& name, double& variable, Distribution& parameter){
            if(started) (*stream)<<"\t";
            (*stream)<<variable;
            if(not started) started = true;
        }
    };

    void variables(std::ostream& stream, Model& model){
        ValueWriter valueWriter;
        valueWriter.stream = &stream;
        bind(model,valueWriter);
    }

    void variables(std::ostream& stream, const std::vector<double>& parameters){
        Model model;
        set(model,parameters);
        variables(stream,model);
    }


    struct PriorWriter {
        std::ostream* stream;
        bool started = false;

        template<class Parameter>
        void operator()(const std::string& name, double& variable, Parameter& parameter){
            double lower = parameter.prior.minimum();
            double upper = parameter.prior.maximum();
            double step = (upper-lower)/500;
            for(double value=lower;value<=upper;value+=step){
                (*stream)<<name<<"\t"<<value<<"\t"<<parameter.prior.pdf(value)<<"\t"<<std::endl;
            }
        }
    };

    void prior_pdfs(const std::string& filename){
        std::ofstream file(filename);
        file<<"name\tvalue\tpdf"<<std::endl;
        
        PriorWriter writer;
        writer.stream = &file;
        
        Model model;
        bind(model,writer);
    }
    */
};

} // namespace Estimation
} // namespace Fsl
#endif