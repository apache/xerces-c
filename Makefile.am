SUBDIRS						= lib src obj tests samples

ACLOCAL_AMFLAGS				= -I m4

check:
	-rm -f tempResult
	export PATH=${abs_top_builddir}/samples:${abs_top_builddir}/tests:"${PATH}" && cd ${top_srcdir} && perl scripts/sanityTest.pl 2>&1 | ${SED} 's/ *[0-9][0-9]*  *ms */{timing removed}/' 1> ${abs_top_builddir}/tempResult
	diff -q tempResult ${top_srcdir}/scripts/sanityTest_ExpectedResult.log
	
test-suite:
	cd swig/perl && perl Makefile.PL && make && make test
	touch $@
