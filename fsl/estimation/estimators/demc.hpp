#pragma once

#include <functional>

#include <boost/container/flat_set.hpp>

namespace Fsl {
namespace Estimation {
namespace Estimators {

class DEMC : public Estimator<DEMC> {
public:
    
    double outbreeding = 0.1;
    double crossing = 0.9;
    double blending = 1;

    double best = -INFINITY;
    double worst = INFINITY;
    double mean = NAN;
    double acceptance = NAN;

    Uniform chance = {0,1};

private:

    void update_(void){
        double sum = 0;
        best = -INFINITY;
        worst = INFINITY;
        for(auto sample : samples){
            double likelihood = sample.likelihood();
            sum += likelihood;
            best = std::max(likelihood,best);
            worst = std::min(likelihood,worst);
        };
        mean = sum/samples.rows();
    }

    /**
     * Select a sample to breed from in probability to its
     * relative likelihood
     */
    Sample select_(void){
        return samples.random();
        /*
        while(true){
            Sample sample = samples.random();
            if(best-worst<=0) return sample;
            else {
                double fitness = (sample.likelihood()-worst)/(best-worst);
                if(chance.random()<fitness) return sample;
            }
        };
        */
    }

    double likelihood_(int trial,const Values& candidate,std::ostream& errors_file){
        double like = NAN;
        try {            
            like = likelihood(candidate);
        } catch(const std::exception& e){
            errors_file<<trial<<"\t"<<e.what();
            for(auto par : candidate) errors_file<<"\t"<<par;
            errors_file<<std::endl;
        } catch(...){
            errors_file<<trial<<"\t"<<"\"Unknown error\"";
            for(auto par : candidate) errors_file<<"\t"<<par;
            errors_file<<std::endl;
        }
        return like;
    }
       
public:

    //! @brief Create a set of replicates for evaluations
    //!
    //! This method must set both *parameters* and *states*.
    void run(uint size=1000, uint trials=1e6) {       
        std::ofstream log_file;
        if(log) log_file.open(directory+"/log.tsv");

        std::ofstream errors_file;
        if(errors) errors_file.open(directory+"/errors.tsv");

        uint columns = samples.columns();
        uint accepted = 0;

        // For each trial...
        for(uint trial=0;trial<trials;trial++){
            update_();

            uint rows = samples.rows();
                
            Values candidate(columns);
            double likelihood = NAN;
            bool accept = false;
            if(rows<size){
                candidate = initial();
                likelihood = likelihood_(trial,candidate,errors_file);
                if(std::isfinite(likelihood)){
                    accept = true;
                }
            } else {
                Sample target = select_();
                Values donor(columns);
                if(chance.random()<outbreeding){
                    donor = initial();
                } else {
                    Sample a = select_();
                    Sample b = select_();
                    Sample c = select_();
                    for(uint column=0;column<columns;column++){
                        donor[column] = a[column] + blending*(b[column]-c[column]);
                    }
                }
                uint which = chance.random() * columns;
                for(uint column=0;column<columns;column++){
                    if(column==which or chance.random()<crossing){
                        candidate[column] = donor[column];
                    }
                    else candidate[column] = target[column];
                }
                candidate = restrict(candidate);
                likelihood = likelihood_(trial,candidate,errors_file);
                if(std::isfinite(likelihood)){
                    double ratio = std::exp(likelihood-target.likelihood());
                    if(chance.random()<ratio){
                        accept = true;
                        samples.remove(target);
                    }
                }    
            }
            
            if(accept){
                samples.append(candidate,likelihood);
                accepted++;
                update_();
            }

            // Log
            if(trial%log==0){
                if(log_file.tellp()==0) log_file<<"trial\trows\tworst\tmean\tbest\tlast\tacceptance"<<std::endl;
                double acceptance = log>0?accepted/double(log):NAN;
                log_file<<trial<<"\t"<<rows<<"\t"<<worst<<"\t"<<mean<<"\t"<<best<<"\t"<<likelihood<<"\t"<<acceptance<<std::endl;
                accepted = 0;
            }
            // Store
            if(trial%store==0){
                write();
            }
        }
    }
};

}
}
}
