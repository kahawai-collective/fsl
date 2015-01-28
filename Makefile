all:
	$(MAKE) -C fsl
	$(MAKE) -C docs
	

BOOST_VERSION := 1_57_0

requires/boost_$(BOOST_VERSION).tar.bz2:
	@mkdir -p requires
	wget --no-check-certificate -O $@ http://prdownloads.sourceforge.net/boost/boost_$(BOOST_VERSION).tar.bz2

requires/boost: requires/boost_$(BOOST_VERSION).tar.bz2
	tar --bzip2 -xf $< -C requires
	mv requires/boost_$(BOOST_VERSION) requires/boost
	cd requires/boost && ./bootstrap.sh --with-libraries=filesystem,system,test && ./b2

STENCILA_VERSION := 0.9

requires/stencila-$(STENCILA_VERSION).tar.gz:
	@mkdir -p requires
	wget --no-check-certificate -O $@ https://github.com/stencila/stencila/releases/download/$(STENCILA_VERSION)/stencila-linux-x86_64-$(STENCILA_VERSION).tar.gz

requires/stencila: requires/stencila-$(STENCILA_VERSION).tar.gz
	tar -xf $< -C requires
	@touch $@

requires: requires/boost requires/stencila
