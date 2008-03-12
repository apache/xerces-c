SUBDIRS = util

test-suite:
	make -C util
	cd perl && perl Makefile.PL
	make -C perl test
	touch $@
