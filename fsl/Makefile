# Makefile for FSL.
#
# Currently the only tasks are clean and tests.  Other tasks will be written for running simulations etc
# Due to the highly nested nature of the FiSL directory tree, with test files in each subdirectory, rather than
# using recursive make, file pattern matching is used. See ["Recursive Make considered harmful"](http://aegis.sourceforge.net/auug97.pdf)
#
# At some stage it may be good to compile all tests together in a single executable. If test stats and code coverage stats are to be recorded 
# that is probably essential
#
# Remember to use parallel builds for speedy compilation and running of all tests
#    make -j 5 clean tests

include Makefile.include

# Get a list of FSL header source files (files with extension *.hpp)
FSL_HEADERS := $(shell find -name "*.hpp")
# Get a list of tests (files with extension *.cxx)
FSL_TESTS_CXX := $(shell find -name "*.cxx")
# Create correspoding list of test executables
FSL_TESTS_EXE := $(patsubst %.cxx,%.test,$(FSL_TESTS_CXX))
# Create correspoding list of test outputs
FSL_TESTS_OUT := $(patsubst %.cxx,%.out,$(FSL_TESTS_CXX))

tests : $(FSL_TESTS_OUT)

# Compile a single test. The .test file extension is used to indicate that a C++ file should be compiled into a test executable
%.test : %.cxx $(FSL_HEADERS)
	$(FSL_COMPILE_PROD) -DFSL_TEST_SINGLE -o $@ $< $(FSL_LIB_DIRS) $(FSL_LIBS) -lboost_unit_test_framework

%.debug : %.cxx $(FSL_HEADERS)
	$(FSL_COMPILE_DEBUG) -DFSL_TEST_SINGLE -o $@ $< $(FSL_LIB_DIRS) $(FSL_LIBS) -lboost_unit_test_framework

# Run a single test and redirect stdout and sterr to both the console and to a file with extension .out
%.out : %.test
	./$< 2>&1 | tee $@

# Compile a production executable
%.exe : %.cpp $(FSL_HEADERS)
	$(FSL_COMPILE_PROD) -o $@ %.cpp $(FSL_LIB_DIRS) $(FSL_LIBS)

all : tests

clean:
	rm -f $(FSL_TESTS_EXE) $(FSL_TESTS_OUT)
