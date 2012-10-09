
BOOST_VERSION = 1_50_0

all: boost 

.PHONY: boost 

####################################################
# include and lib directories 
# 
# Where soft links to C++ files and compiles libraries go

include: 
	mkdir -p include
	
lib:
	mkdir -p lib

####################################################
# boost
#

boost_$(BOOST_VERSION).tar.bz2:
	wget http://prdownloads.sourceforge.net/boost/boost_$(BOOST_VERSION).tar.bz2
	
boost_$(BOOST_VERSION): boost_$(BOOST_VERSION).tar.bz2
	tar --bzip2 -xf boost_$(BOOST_VERSION).tar.bz2

# Boost is made with some options to overide defaults
#  --prefix=.  - so that boost installs into its own directory  (boost_$(BOOST_VERSION))
# link=static - so that get statically compiled instead of dynamically compile libraries
boost_$(BOOST_VERSION)/lib/libboost_system.a: boost_$(BOOST_VERSION)
	cd boost_$(BOOST_VERSION); \
	./bootstrap.sh; \
	./b2 --prefix=. link=static install;
	
# Link to boost include directory and each of the statically compiled libraries
boost: boost_$(BOOST_VERSION)/lib/libboost_system.a include lib
	ln -sfT ../boost_$(BOOST_VERSION)/include/boost include/boost
	for file in $$(ls boost_$(BOOST_VERSION)/lib/*.a); do ln -sf ../$$file lib; done

	