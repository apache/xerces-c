SUBDIRS = 

AM_CPPFLAGS =						-I${top_srcdir}/src

noinst_LTLIBRARIES =				libutil.la

libutil_ladir =					util/
nobase_libutil_la_HEADERS =			${util_headers}
libutil_la_SOURCES =					${util_sources}

util_sources = \
	util/XMLExceptionHandler.cpp

util_headers = \
	util/XMLExceptionHandler.hpp

#
# Override generated rules to provide prettier make rules
#
# Warning: If Automake changes sufficiently, these rules may need to
#          be regenerated from the (new) default output of Automake
#
if XERCES_PRETTY_MAKE
.cpp.o:
@am__fastdepCXX_TRUE@	@depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`; \
@am__fastdepCXX_TRUE@	if $(CXXCOMPILE) -MT $@ -MD -MP -MF "$$depbase.Tpo" -c -o $@ $<; \
@am__fastdepCXX_TRUE@	then mv -f "$$depbase.Tpo" "$$depbase.Po"; else rm -f "$$depbase.Tpo"; exit 1; fi
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	@source='$<' object='$@' libtool=no @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(CXXCOMPILE) -c -o $@ $<

.cpp.obj:
@am__fastdepCXX_TRUE@	@depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.obj$$||'`; \
@am__fastdepCXX_TRUE@	if $(CXXCOMPILE) -MT $@ -MD -MP -MF "$$depbase.Tpo" -c -o $@ `$(CYGPATH_W) '$<'`; \
@am__fastdepCXX_TRUE@	then mv -f "$$depbase.Tpo" "$$depbase.Po"; else rm -f "$$depbase.Tpo"; exit 1; fi
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	@source='$<' object='$@' libtool=no @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(CXXCOMPILE) -c -o $@ `$(CYGPATH_W) '$<'`

.cpp.lo:
@am__fastdepCXX_TRUE@	@depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.lo$$||'`; \
@am__fastdepCXX_TRUE@	if $(LTCXXCOMPILE) -MT $@ -MD -MP -MF "$$depbase.Tpo" -c -o $@ $<; \
@am__fastdepCXX_TRUE@	then mv -f "$$depbase.Tpo" "$$depbase.Plo"; else rm -f "$$depbase.Tpo"; exit 1; fi
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	@source='$<' object='$@' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(LTCXXCOMPILE) -c -o $@ $<

PRETTY_MAKE=${top_srcdir}/config/pretty-make
PRETTY_COMPILE=${PRETTY_MAKE} Compiling
endif

CXXCOMPILE = ${PRETTY_COMPILE} \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = ${PRETTY_COMPILE}  \
	$(LIBTOOL) --quiet --tag=CXX --mode=compile \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)

