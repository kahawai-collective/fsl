#pragma once

namespace Fsl {
namespace Estimation {
namespace Estimators {

template<class Derived>
class Estimator : public Polymorph<Derived> {
public:

    using Polymorph<Derived>::derived;

    Samples samples;

    /**
     * Problem definition
     * 
     * We use [std::function](http://en.cppreference.com/w/cpp/utility/functional/function) 
     * because these are polymorphic and able to store normal functions as well as captured-lambdas
     * amongst others
     * 
     * @{
     */
    
    std::function<Values (void)> initial;
    std::function<Values (const Values&)> restrict;
    std::function<double (const Values&)> likelihood;
    
    /*
     * @}
     */

    /**
     * Home directory
     */
    std::string directory = "estimator";

    /**
     * Write log every `log` iterations
     */
    unsigned int log  = 100;

    /**
     * Write errors to file?
     */
    bool errors = true;

    /**
     * Write samples every `store` iterations
     */
    unsigned int store = 1000;


    Estimator(const std::string& directory = "estimator"):
        directory(directory){
        boost::filesystem::create_directories(directory);
    }

    Derived& parameters(const std::vector<std::string>& names) {
        samples.names(names);
        return derived();
    }

    Derived& read(const std::string& path=""){
        std::string filename;
        if(path.length()==0) filename = directory+"/samples.tsv";
        else filename = path;
        samples.read(filename);
        return derived();
    }

    Derived& write(const std::string& path=""){
        std::string filename;
        if(path.length()==0) filename = directory+"/samples.tsv";
        else filename = path;
        samples.write(filename);
        return derived();
    }

};

}
}
}
