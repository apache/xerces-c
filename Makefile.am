SUBDIRS						= lib src obj tests samples

ACLOCAL_AMFLAGS				= -I m4

test-suite:
	cd swig/perl && perl Makefile.PL && make && make test
	touch $@
