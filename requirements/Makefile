
BOOST_VERSION = 1_50_0
DOXYGEN_VERSION = 1.8.2

all: boost stencila-library

####################################################
# include and lib directories 
# 
# These directories are where soft links to C++ header files and compiled libraries go

include: 
	mkdir -p include
	
lib:
	mkdir -p lib

####################################################
# boost

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
	

####################################################
# stencila
	
 # Currently just using git to clone the stencila repo. An alternative, if a specific version was required, would
 # be to get a zip ball
# 	wget -O stencila-master.zip https://github.com/stencila/stencila/archive/master.zip
#	unzip stencila-master.zip
stencila:
	git clone https://github.com/stencila/stencila.git

# Note that boost is not built as a stencila requirement since it is already a fisl requirement
# Instead, just provide a softlink to the fisl copy of boost
stencila-library: stencila include
	cd stencila; git pull
	cd stencila/cpp/requirements; make pugixml rapidjson smhasher sqlite
	cd stencila/cpp/requirements; ln -sFT ../../../../include/boost/ include/boost
	cd stencila/cpp; make library
	ln -sfT ../stencila/cpp/stencila include/stencila

####################################################
# Doxygen http://www.stack.nl/~dimitri/doxygen/
# Only required if generating documentation HTML. Not required for library use

doxygen-$(DOXYGEN_VERSION):
	wget http://ftp.stack.nl/pub/users/dimitri/doxygen-$(DOXYGEN_VERSION).src.tar.gz
	
doxygen: doxygen-$(DOXYGEN_VERSION)
	tar -xvf doxygen-$(DOXYGEN_VERSION).src.tar.gz
	cd doxygen-$(DOXYGEN_VERSION); \
	./configure; \
	make; \
	sudo make install ;\
