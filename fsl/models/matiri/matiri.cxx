#ifdef FSL_TEST_SINGLE
    #define BOOST_TEST_MODULE tests
#endif
#include <boost/test/unit_test.hpp>

#include <fsl/models/matiri/matiri.hpp>

BOOST_AUTO_TEST_SUITE(matiri)

    using namespace Fsl::Models;   

    BOOST_AUTO_TEST_CASE(one){
        class MyModel : public Matiri::Model<MyModel,30,2,3> {
        public:
		    void defaults(void){
		        recruitment_relation.r0 = 1e6;
		        recruitment_relation.steepness = 0.9;

		        recruitment_variation
		        	.sd(0.6);

		        sex_ratio = 0.5;

		        mortalities.fun(male).value(0.3);
		        mortalities.fun(female).value(0.3);

		        lengths.fun(male).k = 0.3;
		        lengths.fun(male).linf = 100;
		        lengths.fun(male).t0 = 0;

		        lengths.fun(female).k = 0.3;
		        lengths.fun(female).linf = 100;
		        lengths.fun(female).t0 = 0;

		        weights.fun(male)
		        	.a(5.32e-9)
		        	.b(3.35);
		        weights.fun(female)
		        	.a(5.32e-9)
		        	.b(3.35);

		        maturities.fun(male).inflection = 5;
		        maturities.fun(male).steepness = 0.001;
		        maturities.fun(female).inflection = 5;
		        maturities.fun(female).steepness = 0.001;

		        for(auto sector : sectors){
		        	for(auto sex : sexes){
				        selectivities.fun(sector,sex).inflection_1 = 10;
						selectivities.fun(sector,sex).inflection_2_delta = 5;
						selectivities.fun(sector,sex).steepness_1 = 10;
						selectivities.fun(sector,sex).steepness_2 = 10;
					}
				}
		    }
        };
        MyModel model;
        model.initialise();
        model.years(1950,2013);
    }

BOOST_AUTO_TEST_SUITE_END()
