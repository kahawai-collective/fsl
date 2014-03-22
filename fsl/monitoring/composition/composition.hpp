#pragma once

#include <fstream>
#include <array>

namespace Fsl {
namespace Monitoring {
namespace Composition {

template<
    unsigned int Bins
>
class Sample {
public:
    
    static const unsigned int bins = Bins;
    double size;
    std::array<double,Bins> observeds;
    std::array<double,Bins> expecteds;

    Sample(void):
        size(0){
    }
    
    Sample(const double& size, const std::vector<double>& observed):
        size(size){
        assert(observed.size()==bins);
        for(unsigned int bin=0;bin<bins;bin++) observeds[bin] = observed[bin];
    }
    
    void expected(const std::vector<double>& expected){
        assert(expected.size()==bins);
        for(unsigned int bin=0;bin<bins;bin++) expecteds[bin] = expected[bin];
    }
    
    double likelihood(void) const {
        double a = 0;
        double b = 0;
        double n_ = std::min(size,1000.0);
        for(unsigned int bin=0;bin<bins;bin++) {
            const double& o = observeds[bin];
            const double& e = expecteds[bin];
            double e_ = (1-e)*e+0.1/bins;
            a += std::log(e_);
            double c = -std::pow(o-e,2)/(2*e_/n_);
            b += std::log(std::exp(c)+0.01);
        }
        return -(0.5*a-b);
    }
    
};


template<
    unsigned int Bins,
    class Key = int
>
class Samples {
public:
    typedef Sample<Bins> SampleType;
    
    std::map<Key,SampleType> samples;
    
    void observed(const Key& key, const int size, const std::vector<double>& proportions){
        SampleType sample(size,proportions);
        samples[key] = sample;
    }
    
    void observeds(const std::vector<std::pair<Key,SampleType>>& values){
        for(auto& sample : samples) observed(sample.first,sample.second);
    }
    
    void expected(const Key& key, const std::vector<double>& proportions){
        samples[key].expected(proportions);
    }

    double likelihood(void) const {
        double likelihood = 0;
        for(auto& sample : samples) likelihood += sample.second.likelihood();
        return likelihood;
    }

    void observeds_output(const std::string& filename){
        std::ofstream file(filename);
        file<<"key\tbin\tobs\n";
        for(auto& sample : samples) {
            for(unsigned int bin=0;bin<sample.second.observeds.size();bin++){
                file<<sample.first<<"\t"<<bin<<"\t"<<sample.second.observeds[bin]<<"\n";
            }
        }
    }

    void stream(std::ostream& stream,const std::string& prefix=""){
        for(auto& sample : samples) {
            for(unsigned int bin=0;bin<SampleType::bins;bin++){
                if(prefix.length()>0) stream<<prefix<<"\t";
                stream
                    <<sample.first<<"\t"
                    <<bin<<"\t"
                    <<sample.second.observeds[bin]<<"\t"
                    <<sample.second.expecteds[bin]<<"\n";
            }
        }
    }
};

}
}
}
