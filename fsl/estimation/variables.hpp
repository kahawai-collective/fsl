#pragma once

#include <boost/regex.hpp>

#include <stencila/reflector.hpp>

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

template<
    class Distribution
>
class Variate : public Distribution, public Reflector<Variate<Distribution>> {
private:
    double value_ = NAN;

public:

    Variate(void):
        Distribution(){
    }

    Variate(const double& value):
        Distribution(value),
        value_(value){
    }

    template<typename... Args>
    Variate(const double& value, const Args&... args):
        Distribution(args...),
        value_(value){
    }

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror
            .data(value_,"value");
        Distribution::reflect(mirror);
    }

    double value(void) const{
        return value_;
    }

    Variate& value(const double& value);

    operator double(void) const {
        return value();
    }

    void operator=(const double& other){
        value(other);
    }

    #define OP_(op) void operator op(const double& other){ value_ op other; }
        OP_(+=)
        OP_(-=)
        OP_(*=)
        OP_(/=)
    #undef OP_

    bool valid(void) const {
        return std::isfinite(value()) and Distribution::valid();
    }

    bool free(void) const;

    double restrict(const double& value){
        if(value<Distribution::minimum()) return Distribution::minimum();
        else if(value>Distribution::maximum()) return Distribution::maximum();
        else return value;
    }

    double likelihood(void) const {
        if(valid()) return Distribution::likelihood(value());
        else return 0;
    }

}; // class Variate

template<class Distribution>
bool Variate<Distribution>::free(void) const {
    return true;
}

template<>
bool Variate<Fixed>::free(void) const {
    return false;
}

template<class Distribution>
Variate<Distribution>& Variate<Distribution>::value(const double& value){
    if(not std::isfinite(value)) throw std::runtime_error(str(boost::format("Value <%s> is not finite.")%value));
    if(value<Distribution::minimum()) throw std::runtime_error(str(boost::format("Value <%s> is less than minimum <%s> for variate.")%value%Distribution::minimum()));
    else if(value>Distribution::maximum()) throw std::runtime_error(str(boost::format("Value <%s> is greater than maximum <%s> for variate.")%value%Distribution::maximum()));
    else value_ = value;
    return *this;
}

template<>
Variate<Fixed>& Variate<Fixed>::value(const double& value){
    // Using the default definition of this function can cause problems
    // for `Variate<Fixed>`s because (due to [machine epsilon](http://en.wikipedia.org/wiki/Machine_epsilon)?)
    // a value that is equal to the fixed value can be evaluated as less than or greater than the value.
    value_ = value;
    return *this;
}

class Samples;

class Sample {
private:

    const Samples& samples_;
    uint row_;

public:

    Sample(const Samples& samples,uint row):
        samples_(samples),
        row_(row){        
    }

    uint index(void) const {
        return row_;
    }

    uint columns(void) const;

    /**
     * Check the sample has a column with name
     * 
     * @param  name Variate name
     */
    bool has(const std::string& name) const;

    /**
     * Get a variate value from the sample
     * 
     * @param  Index of the variate
     */
    double get(uint index) const;

    /**
     * Get a variate value from the sample
     * 
     * @param  name Variate name
     */
    double get(const std::string& name) const;

    /**
     * Get likelihood of sample
     */
    double likelihood(void) const;


    double operator[](uint index) const {
        return get(index);
    }

    /**
     * @name Iterator interface
     *
     * Operator overloads necessary for acting as an
     * iterator
     * 
     * @{
     */

    const Sample& operator*() const { 
        return *this;
    }

    Sample& operator++() {
        ++row_;
        return *this;
    }

    bool operator==(const Sample& other) const {
        return row_ == other.row_;
    }

    bool operator!=(const Sample& other) const {
        return row_ != other.row_;
    }

    /**
     * @}
     */

};

typedef std::vector<double> Values;

class Samples {
private:

    uint rows_ = 0;
    
    struct Cell_ {
        Values values;
        double likelihood;

        Cell_(const Values& values,const double& likelihood = NAN):
            values(values),
            likelihood(likelihood){
        }
    };
    std::vector<Cell_> cells_;

    std::vector<std::string> names_;

public:

    uint rows(void) const {
        return cells_.size();
    }

    uint columns(void) const {
        return names_.size();
    }

    bool has(const std::string& name) const {
        auto iter = std::find(names_.begin(),names_.end(),name);
        if(iter != names_.end()) return true;
        else return false;
    }

    double get(uint row, uint column) const {
        return cells_[row].values[column];
    }

    double get(uint row, const std::string& name) const {
        auto iter = std::find(names_.begin(),names_.end(),name);
        if(iter != names_.end()){
            uint column = iter-names_.begin();
            return get(row,column);
        }
        else return NAN;
    }

    std::vector<std::string> names(void) const {
        return names_;
    }

    Samples& names(const std::vector<std::string>& names){
        names_ = names;
        return *this;
    }

    double likelihood(uint row) const {
        return cells_[row].likelihood;
    }

    Sample operator[](uint index) const {
        return Sample(*this,index);
    }

    Sample begin(void) const {
        return Sample(*this,0);
    }

    Sample end(void) const {
        return Sample(*this,rows());
    }    

    Sample random(void) const {
        uint row = std::rand()%rows();
        return Sample(*this,row);
    }

    Samples& append(const Values& values,double likelihood=NAN){
        cells_.push_back(Cell_(values,likelihood));
        return *this;
    }

    Samples& remove(const Sample& sample){
        return remove(sample.index());
    }

    Samples& remove(uint index){
        cells_.erase(cells_.begin()+index);
        return *this;
    }

    /**
     * Read a file of whitespace separated values
     * 
     * @param  filename [description]
     * @return          [description]
     */
    Samples& read(const std::string& filename,bool header=true){
        std::ifstream file(filename);
        std::string line;

        boost::regex rgx("\\s+");
        boost::sregex_token_iterator end;
            
        // Read in header   
        std::getline(file, line);
        boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
        for( ; iter != end; ++iter) names_.push_back(*iter);
        // Remove the last name (which should be likelihood)
        names_.pop_back();

        while(true){
            std::getline(file, line);
            if(not file.good()) break;
            boost::sregex_token_iterator iter(line.begin(),line.end(),rgx,-1);
            Values values;
            for( ; iter != end; ++iter){
                std::string string = *iter;
                double value;
                if(string=="NA") value = NAN;
                else value = boost::lexical_cast<double>(string);
                values.push_back(value);
            }
            double likelihood = values.back();
            values.pop_back();
            cells_.push_back(Cell_(values,likelihood));
        }

        file.close();

        return *this;
    }

    void write(const std::string& filename){
        std::ofstream file(filename);

        for(auto name : names_) file<<name<<"\t";
        file<<"likelihood"<<std::endl;
    
        for(auto row : cells_){
            for(auto value : row.values) file<<value<<"\t";
            file<<row.likelihood<<std::endl;
        }

        file.close();
    }

};

uint Sample::columns(void) const {
    return samples_.columns();
}

bool Sample::has(const std::string& name) const {
    return samples_.has(name);
}

double Sample::get(uint index) const {
    return samples_.get(row_,index);
}

double Sample::get(const std::string& name) const {
    return samples_.get(row_,name);
}

double Sample::likelihood() const {
    return samples_.likelihood(row_);
}

template<
    class Distribution
>
class Variable : public Variate<Distribution> {
public:

    template<typename... Args>
    Variable(const Args&... args):
        Variate<Distribution>(args...){
    }

}; // class Variable

template<
    class Distribution,
    class D1 = Singular1,
    class D2 = Singular2,
    class D3 = Singular3,
    class D4 = Singular4,
    class D5 = Singular5,
    class D6 = Singular6,
    class D7 = Singular7,
    class D8 = Singular8,
    class D9 = Singular9,
    class D10 = Singular10
>
class Variables : public Array<Variate<Distribution>,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> {
private:

    typedef Variate<Distribution> VariateType;
    typedef Array<Variate<Distribution>,D1,D2,D3,D4,D5,D6,D7,D8,D9,D10> ArrayType;

public:

    double likelihood(void) const {
        double sum = 0;
        for(auto variate : *this) sum += variate.likelihood();
        return sum;
    }

}; // class Variables


template<
	class Derived
>
class Set : public Reflector<Derived> {
public:

    std::string path = ".";
    std::ostream* stream_ = nullptr;

    Set(const std::string& path="."):
        path(path){
        boost::filesystem::create_directories(path);
    }

    ~Set(void){
        if(stream_) delete stream_;
    }

    using Polymorph<Derived>::derived;

    template<class DerivedMirror>
    struct SetMirror : Mirror<DerivedMirror> {  

        template<class Distribution>
        DerivedMirror& data(Variable<Distribution>& variable, const std::string& name){
            derived().variate(variable,name);
            return derived();
        }

        template<class Distribution,class... Indices>
        DerivedMirror& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            for(uint index=0;index<variables.size();index++){
                derived().variate(variables[index],name+variables.subscript(index,true));
            }
            return derived();
        }         
    };

public:

    /**
     * Get the number of variates in the `Set`
     */
    uint count(void) {
        Count_ mirror;
        derived().reflect(mirror);
        return mirror.count;
    }

private:

    struct Count_ : SetMirror<Count_> {
        uint count = 0;

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            count++;
        }       
    };

public:

    /**
     * Get the names of variates in the `Set`
     */
    std::vector<std::string> names(bool all=true) {
        Names_ mirror(all);
        derived().reflect(mirror);
        return mirror.names;
    }

private:

    struct Names_ : SetMirror<Names_> {
        std::vector<std::string> names;
        bool all;

        Names_(bool all):
            all(all){}

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            if(all or variate.free()) names.push_back(name);
        }       
    };

public:

    /**
     * Load the `Set` from a `Sample` of variates
     * 
     * @param  sample A `Sample` to load variate values from
     */
    Derived& load(const Sample& sample){
        LoadSample_ mirror(sample);
        derived().reflect(mirror);
        return derived();
    }

private:

    struct LoadSample_ : SetMirror<LoadSample_> {
        const Sample& sample;

        LoadSample_(const Sample& sample):
            sample(sample){
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            try {
                if(sample.has(name)){
                    double value = sample.get(name);
                    if(not std::isfinite(value)) throw std::runtime_error("`Set::load` : When loading variate <"+name+"> from sample, value was not finite");
                    variate = value;
                }
            } catch(const std::exception& exc){
                throw std::runtime_error("`Set::load` : Error when loading variate <"+name+"> from sample : "+exc.what());
            }
        }       
    };

public:

    /**
     * Load the `Set` from a std::vector<double>
     * 
     * @param  sample A `Sample` to load variate values from
     */
    Derived& load(const std::vector<double>& vector){
        LoadVector_ mirror(vector);
        derived().reflect(mirror);
        return derived();
    }

private:

    struct LoadVector_ : SetMirror<LoadVector_> {
        const std::vector<double>& vector;
        uint index = 0;

        LoadVector_(const std::vector<double>& vector):
            vector(vector){
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            try {
                variate = vector[index++];
            } catch(const std::exception& exc){
                throw std::runtime_error("Error when loading variate <"+name+"> from vector: "+exc.what());
            }
        }       
    };


public:
    void read(void){
        Reader reader(path);
        derived().reflect(reader);
    }
    class Reader : public Mirror<Reader> {
    private:
        std::string path_;

        std::string filename_(const std::string& name){
            return path_ + "/" + name + ".in.tsv";
        }

    public:
        Reader(const std::string& path):
            path_(path){
        }

        template<class Distribution>
        Reader& data(Variable<Distribution>& variable, const std::string& name){
            return *this;
        }

        template<class Distribution,class... Indices>
        Reader& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            std::string filename = filename_(name);
            if(boost::filesystem::exists(filename)){
                std::cout<<"Reading <"<<filename_(name)<<">\n";
                variables.read(filename,true);
            } else {
                std::cout<<"Warning file not found <"<<filename_(name)<<">\n";
            }
            return *this;
        }    
    };

    void write(void){
        write_list();
        write_pdfs();
        write_files();
        write_likelihoods();
    }

    void write_list(void){
        ListWriter writer(path);
        derived().reflect(writer);
    }
    class ListWriter : public Mirror<ListWriter> {
    private:
        std::ofstream file_;
    public:
        ListWriter(const std::string& path):
            file_(path+"/list.tsv"){
            file_<<"name\tvalue\tminimum\tmaximum\tmean\tsd\n";
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            file_<<name<<"\t"<<variate.value()<<"\t"<<variate.minimum()<<"\t"<<variate.maximum()<<"\t"<<variate.mean()<<"\t"<<variate.sd()<<"\n";
        }

        template<class Distribution>
        ListWriter& data(Variable<Distribution>& variable, const std::string& name){
            variate(variable,name);
            return *this;
        }

        template<class Distribution,class... Indices>
        ListWriter& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            for(uint index=0;index<variables.size();index++){
                variate(variables[index],name+variables.subscript(index,true));
            }
            return *this;
        }   
    };

    void write_pdfs(void){
        PdfWriter writer(path);
        derived().reflect(writer);
    }
    class PdfWriter : public Mirror<PdfWriter> {
    private:
        std::ofstream file;
    public:
        PdfWriter(const std::string& path):
            file(path+"/pdfs.tsv"){
            file<<"name\tvalue\tpdf\n";
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            double min = variate.minimum();
            if(not std::isfinite(min)) min = variate.quantile(0.001);
            double max = variate.maximum();
            if(not std::isfinite(max)) max = variate.quantile(0.999);
            if(std::isfinite(min) and std::isfinite(max)){
                double step = (max-min)/100;
                if(step<=0) step = 1;
                for(double value=min;value<=max;value+=step){
                    file<<name<<"\t"<<value<<"\t"<<variate.pdf(value)<<"\t"<<std::endl;
                }
            } else {
                file<<name<<"\t"<<NAN<<"\t"<<NAN<<"\t"<<std::endl;
            }
        }

        template<class Distribution>
        PdfWriter& data(Variable<Distribution>& variable, const std::string& name){
            variate(variable,name);
            return *this;
        }

        template<class Distribution,class... Indices>
        PdfWriter& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            for(uint index=0;index<variables.size();index++){
                variate(variables[index],name+variables.subscript(index,true));
            }
            return *this;
        }   
    };

    void write_files(void){
        FileWriter writer(path);
        derived().reflect(writer);
    }
    class FileWriter : public Mirror<FileWriter> {
    private:
        std::string path_;

        std::string filename_(const std::string& name){
            return path_ + "/" + name + ".out.tsv";
        }

    public:
        FileWriter(const std::string& path):
            path_(path){
        }

        template<class Distribution>
        FileWriter& data(Variable<Distribution>& variable, const std::string& name){
            return *this;
        }

        template<class Distribution,class... Indices>
        FileWriter& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            std::cout<<"Writing <"<<filename_(name)<<">\n";
            variables.write(filename_(name),true);
            return *this;
        }   
    };

    void write_likelihoods(void){
        LikelihoodsWriter writer(path);
        derived().reflect(writer);
    }
    class LikelihoodsWriter : public Mirror<LikelihoodsWriter> {
    private:
        std::ofstream file_;
    public:
        LikelihoodsWriter(const std::string& path):
            file_(path+"/likelihoods.tsv"){
            file_<<"name\tlikelihood\n";
        }

        template<class Distribution>
        LikelihoodsWriter& data(Variable<Distribution>& variable, const std::string& name){
            file_<<name<<"\t"<<variable.likelihood()<<"\n";
            return *this;
        }

        template<class Distribution,class... Indices>
        LikelihoodsWriter& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            file_<<name<<"\t"<<variables.likelihood()<<"\n";
            return *this;
        }   
    };

        
    Derived& stream(const std::string& filename, const std::vector<std::string>& prefixes){
        stream_ = new std::ofstream(path+"/"+filename);
        for(auto prefix : prefixes) *stream_<<prefix<<"\t";
        *stream_<<"name\tvalue\n";
        return derived();
    }

    template<typename... Args>
    Derived& stream(Args... args){
        if(not stream_) throw std::runtime_error("Streaming has not been initiated");
        Stream_ mirror(*stream_,stream_prefixes_(args...));
        derived().reflect(mirror);
        return derived();
    }

    template<
        typename Arg,
        typename... Args
    >
    std::string stream_prefixes_(Arg arg, Args... args){
        return boost::lexical_cast<std::string>(arg) + "\t" + stream_prefixes_(args...);
    }

    std::string stream_prefixes_(void){
        return "";
    }

    struct Stream_ : SetMirror<Stream_> {
        std::ostream& stream;
        std::string prefixes;

        Stream_(std::ostream& stream, const std::string prefixes):
            stream(stream),
            prefixes(prefixes){
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            stream<<prefixes<<"\t"<<name<<"\t"<<variate.value()<<"\n";
        }   
    };


    double likelihood(void){
        ProbabilityCalculater calcer;
        derived().reflect(calcer);
        return calcer.value;
    }
    struct ProbabilityCalculater : public Mirror<ProbabilityCalculater> {
    public:
        double value = 0;

        template<class Distribution>
        ProbabilityCalculater& data(Variable<Distribution>& variable, const std::string& name){
            value += variable.likelihood();
            return *this;
        }

        template<class Distribution,class... Indices>
        ProbabilityCalculater& data(Variables<Distribution,Indices...>& variables, const std::string& name){
            value += variables.likelihood();
            return *this;
        }   
    };


public:

    /**
     * Get a `Sample` of variate values from the `Set`
     */
    std::vector<double> sample(void){
        Sample_ sample_;
        derived().reflect(sample_);
        return sample_.sample;
    }

private:

    struct Sample_ : SetMirror<Sample_> {
        std::vector<double> sample;

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            sample.push_back(variate.random());
        }       
    };

public:

    /**
     * Restrict variate values to their bounds
     */
    std::vector<double> restrict(const std::vector<double>& values){
        Restrict_ restrict_(values);
        derived().reflect(restrict_);
        return restrict_.restricted;
    }

private:

    struct Restrict_ : SetMirror<Restrict_> {
        const std::vector<double>& supplied;
        std::vector<double> restricted;
        uint index = 0;

        Restrict_(const std::vector<double>& supplied):
            supplied(supplied),
            restricted(supplied.size()){
        }

        template<class Distribution>
        void variate(Variate<Distribution>& variate, const std::string& name){
            restricted[index] = variate.restrict(supplied[index]);
            index++;
        }       
    };

};

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

template<
    class Derived
>
class DataSet : public Set<Derived> {
public:

    DataSet(const std::string& path="data"):
        Set<Derived>(path){
    }
};

} // namespace Estimation
} // namespace Fsl
