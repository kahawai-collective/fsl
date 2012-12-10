# Makefile for FiSL.
#
# Currently the only tasks are clean and tests.  Other tasks will be written for running simulations etc
# Due to the highly nested nature of the FiSL directory tree, with test files in each subdirectory, rather than
# using recursive make file pattern matching is used. See ["Recursive Make considered harmful"](http://aegis.sourceforge.net/auug97.pdf)
#
# At some stage it may be good to compile all tests together in a single executable. If test stats and code coverage stats are to be recorded 
# that is probably essential
#
# Remember to use parrallel builds for speedy compilation and running of all tests
#    make -j 5 clean tests

# Home  directory is the one above this
FISL_HOME := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))/..)

# Include directories
FISL_INCLUDE_DIRS := -I$(FISL_HOME) -I$(FISL_HOME)/requirements/include -I/media/Data/UCI/stencila/source/stencila/cpp/ -I/media/Data/UCI/stencila/source/stencila/cpp/requirements/include

# Library directories
FISL_LIB_DIRS := -L$(FISL_HOME)/requirements/lib -L/media/Data/UCI/stencila/source/stencila/cpp/requirements/lib

# Libraries that need to be included
FISL_LIBS := -lboost_system -lboost_filesystem -lboost_date_time \
				-lsqlite3 -ldl -lpthread

# C++ compile command
FISL_COMPILE := g++ -std=c++0x -g $(FISL_INCLUDE_DIRS)

# Get a list of tests (files with extension *.cxx)
FISL_TESTS_CXX := $(shell find -name *.cxx)
# Create correspoding list of test executables
FISL_TESTS_EXE := $(patsubst %.cxx,%.exe,$(FISL_TESTS_CXX))
# Create correspoding list of test outputs
FISL_TESTS_OUT := $(patsubst %.cxx,%.out,$(FISL_TESTS_CXX))

tests : $(FISL_TESTS_OUT)

# Compile a single test. The .test file extension is used to indicate that a C++ file should be compiled into a test executable
%.exe : %.cxx
	$(FISL_COMPILE) -DFISL_TEST_SINGLE -o $@ $< $(FISL_LIB_DIRS) $(FISL_LIBS) -lboost_unit_test_framework

# Run a single test and redirect stdout and sterr to bothe the console and to a file with extension .out
%.out : %.exe
	./$< 2>&1 | tee $@
	
all : tests

clean:
	rm -f $(FISL_TESTS_EXE) $(FISL_TESTS_OUT)
