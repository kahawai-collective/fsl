include fsl/Makefile.include

all:
	$(MAKE) -C fsl
	$(MAKE) -C docs
	
###################################################################################################

requires: requires/boost requires/stencila

##########################################################################

BOOST_VERSION := 1_57_0

requires/boost_$(BOOST_VERSION).tar.bz2:
	@mkdir -p requires
	wget --no-check-certificate -O $@ http://prdownloads.sourceforge.net/boost/boost_$(BOOST_VERSION).tar.bz2

requires/boost: requires/boost_$(BOOST_VERSION).tar.bz2
	tar --bzip2 -xf $< -C requires
	mv requires/boost_$(BOOST_VERSION) requires/boost
	cd requires/boost && ./bootstrap.sh --with-libraries=filesystem,system,test && ./b2

##########################################################################

STENCILA_VERSION := 0.9

requires/stencila-$(STENCILA_VERSION).tar.gz:
	@mkdir -p requires
	wget --no-check-certificate -O $@ https://github.com/stencila/stencila/releases/download/$(STENCILA_VERSION)/stencila-linux-x86_64-$(STENCILA_VERSION).tar.gz

requires/stencila: requires/stencila-$(STENCILA_VERSION).tar.gz
	tar -xf $< -C requires
	@touch $@

###################################################################################################

# Get a list of tests (files with extension *.cxx)
FSL_TESTS_CXX := $(shell find -name "*.cxx")
# Create correspoding list of test executables
FSL_TESTS_EXE := $(patsubst %.cxx,%.test,$(FSL_TESTS_CXX))
# Create correspoding list of test outputs
FSL_TESTS_OUT := $(patsubst %.cxx,%.out,$(FSL_TESTS_CXX))

tests : $(FSL_TESTS_OUT)

# Compile a single test. The .test file extension is used to indicate that a C++ file should be compiled into a test executable
%.test : %.cxx
	$(FSL_COMPILE_PROD) -o $@ $< $(FSL_LIB_DIRS) $(FSL_LIBS) -lboost_unit_test_framework

%.debug : %.cxx
	$(FSL_COMPILE_DEBUG) -o $@ $< $(FSL_LIB_DIRS) $(FSL_LIBS) -lboost_unit_test_framework

# Run a single test and redirect stdout and sterr to both the console and to a file with extension .out
%.out : %.test
	cd $(shell dirname $<); ./$(shell basename $<) 2>&1 | tee $@

###################################################################################################

FSL_CILA_SRC := $(shell find fsl -name "*.cila")
FSL_CILA_HTML := $(patsubst %.cila,%.html,$(FSL_CILA_SRC))

docss:
	@echo $(FSL_DOCS_CILA)

%/index.html: %/stencil.cila
	cd $(dir $<) && Rscript -e "require(stencila); Stencil('.')$$ render()$$ export('index.html')"


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

all : tests

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
	cd $(shell dirname $<); ./$(shell basename $<) 2>&1 | tee $@

# Compile a production executable
%.exe : %.cpp $(FSL_HEADERS)
	$(FSL_COMPILE_PROD) -o $@ %.cpp $(FSL_LIB_DIRS) $(FSL_LIBS)

clean:
	rm -f $(FSL_TESTS_EXE) $(FSL_TESTS_OUT)
