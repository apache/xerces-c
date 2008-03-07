SUBDIRS = src tests samples

EXTRA_DIST = version.incl \
             config/pretty-make \
             scripts/createBindingsFormatFromHPP.pl \
             scripts/makeStringDefinition.pl \
             scripts/packageBinaries.pl \
             scripts/packageSources.pl \
             scripts/sanityTest.pl \
             scripts/sanityTest_ExpectedResult.log

ACLOCAL_AMFLAGS = -I m4

CLEANFILES = test-results.log

check:
	-rm -f test-results.log
	export PATH=${abs_top_builddir}/samples:${abs_top_builddir}/tests:"${PATH}" && export XERCESC_NLS_HOME=${abs_top_builddir}/src/.libs && cd ${top_srcdir} && perl scripts/sanityTest.pl 2>&1 | ${SED} 's/ *[0-9][0-9]*  *ms */{timing removed}/' 1> ${abs_top_builddir}/test-results.log
	diff test-results.log ${top_srcdir}/scripts/sanityTest_ExpectedResult.log
	
test-suite:
	cd swig/perl && perl Makefile.PL && make && make test
	touch $@
