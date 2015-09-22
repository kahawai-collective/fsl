#pragma once

#include <stencila/mirror-rows.hpp>
using Stencila::Mirrors::RowWriter;

#include <fsl/management/performance.hpp>
#include <fsl/management/procedure.hpp>

namespace Fsl {
namespace Management {

template<class Derived>
class Evaluator : public Polymorph<Derived> {
public:
    using Polymorph<Derived>::derived;

    /**
     * A list of candidate procedures
     */
    std::vector<ProcedureAny> procedures;

    uint first;
    uint start;
    uint last;

    uint replicates = 0;

    Derived& write(void){
        std::ofstream procedures_file("procedures.tsv");
        procedures_file<<"procedure\tsignature\n";
        int index = 0;
        for(auto& procedure : procedures) {
            procedures_file<<index<<"\t"<<procedure.signature()<<"\n";
            index++;
        }
        return derived();
    }

    template<
        class Model,
        class Parameters,
        class Performance
    >
    Derived& run(
        const Model& model,
        const Parameters& parameters,
        const Samples& samples, 
        const Performance& performance
    ){
        // Default is to evaluate all replicates in samples
        if(replicates==0) replicates = samples.rows();

        // Create a local parameter set
        Parameters parameters_ = parameters;
        // Create a performances file stream
        RowWriter performances(
            "performances.tsv",
            {"replicate","sample","procedure"}
        );
        // For each replicate...
        for(uint replicate=0;replicate<replicates;replicate++){
            // Select a random sample
            Sample sample = samples.random();
            // Load sample into parameters
            parameters_.load(sample);
            // Create a starting model state by copying the supplied
            // model and iterating from `first` to `start`
            Model starting = model;
            for(uint time=first;time<start;time++){
                //... set model parameters
                parameters_.set(starting,time);
                //... do `before()` method
                derived().before(replicate,-1,time,starting);
                //... update the model
                starting.update(time);
                //... do `after()` method
                derived().after(replicate,-1,time,starting);
            }
            // Create a random generator seed to be used to ensure the same
            // process and observation error realisations for each candidate
            uint seed = replicate + 13750892;
            // For each candidate...
            for(uint candidate=0;candidate<procedures.size();candidate++){
                // Reset the evaluator
                derived().reset();
                // Reset random seed
                using Math::Probability::Generator;
                Generator.seed(seed);
                // Get and reset the procedure
                ProcedureAny procedure = procedures[candidate];
                procedure.reset();
                // Copy the starting model state and iterate
                // from start to last
                Model model_ = starting;
                // Create a local performance set
                Performance performance_ = performance;
                performance_.initialise(model_);
                for(uint time=start;time<=last;time++){
                    //... set model parameters
                    parameters_.set(model_,time);
                    //... do `before()` method
                    derived().before(replicate,candidate,time,model_);
                    //... operate the procedure
                    procedure.operate(time);
                    //... do `after()` method
                    derived().after(replicate,candidate,time,model_);
                    //... update the model
                    model_.update(time);
                    //... update performance
                    performance_.update(time,model_);
                }
                performance_.finalise(model_);
                performances.write(performance_,replicate,sample.index(),candidate);
            }
        }
        return derived();
    }

    template<
        class Model,
        class Parameters,
        class Performance
    >
    Derived& run(
        const Model& model,
        const Parameters& parameters,
        const Frame& samples, 
        const Performance& performance
    ){
        // Default is to evaluate all replicates in samples
        if(replicates==0) replicates = samples.rows();

        // Create a local parameter set
        Parameters parameters_ = parameters;
        // Create a performances file stream
        RowWriter performances(
            "performances.tsv",
            {"replicate","sample","procedure"}
        );
        // For each replicate...
        for(uint replicate=0;replicate<replicates;replicate++){
            // Select a random sample
            uint sample_index = Uniform(0,samples.rows()).random();
            Frame sample = samples.slice(sample_index);
            // Load sample into parameters
            parameters_.read(sample);
            // Create a starting model state by copying the supplied
            // model and iterating from `first` to `start`
            Model starting = model;
            for(uint time=first;time<start;time++){
                //... set model parameters
                parameters_.set(starting,time);
                //... do `before()` method
                derived().before(replicate,-1,time,starting);
                //... update the model
                starting.update(time);
                //... do `after()` method
                derived().after(replicate,-1,time,starting);
            }
            // Create a random generator seed to be used to ensure the same
            // process and observation error realisations for each candidate
            uint seed = replicate + 13750892;
            // For each candidate...
            for(uint candidate=0;candidate<procedures.size();candidate++){
                // Reset the evaluator
                derived().reset();
                // Reset random seed
                using Math::Probability::Generator;
                Generator.seed(seed);
                // Get and reset the procedure
                ProcedureAny procedure = procedures[candidate];
                procedure.reset();
                // Copy the starting model state and iterate
                // from start to last
                Model model_ = starting;
                // Create a local performance set
                Performance performance_ = performance;
                performance_.initialise(model_);
                for(uint time=start;time<=last;time++){
                    //... set model parameters
                    parameters_.set(model_,time);
                    //... do `before()` method
                    derived().before(replicate,candidate,time,model_);
                    //... operate the procedure
                    procedure.operate(time);
                    //... do `after()` method
                    derived().after(replicate,candidate,time,model_);
                    //... update the model
                    model_.update(time);
                    //... update performance
                    performance_.update(time,model_);
                }
                performance_.finalise(model_);
                performances.write(performance_,replicate,sample_index,candidate);
            }
        }
        return derived();
    }
    
};

} // namespace Management
} // namespace Fsl
