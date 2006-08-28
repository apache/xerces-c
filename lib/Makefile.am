#
# Copyright 2005 The Apache Software Foundation.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# $Id$
#


#
# Specify the convenience libraries for various xerces components
# These are used only during the build process and are not themselves installed
#
noinst_LTLIBRARIES =				libcompat.la

#
# Build convenience libraries for the various xerces components
#
#noinst_libcompat_la_HEADERS =		
libcompat_la_SOURCES =				xerces_compat.c
libcompat_la_LIBADD =				${LTLIBOBJS}

#
# Override generated rules to provide prettier make rules
#
# Warning: If Automake changes sufficiently, these rules may need to
#          be regenerated from the (new) default output of Automake
#
if XERCES_PRETTY_MAKE

.c.lo:
@am__fastdepCXX_TRUE@	@depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.lo$$||'`; \
@am__fastdepCXX_TRUE@	if $(LTCOMPILE) -MT $@ -MD -MP -MF "$$depbase.Tpo" -c -o $@ $<; \
@am__fastdepCXX_TRUE@	then mv -f "$$depbase.Tpo" "$$depbase.Plo"; else rm -f "$$depbase.Tpo"; exit 1; fi
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	@source='$<' object='$@' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CCDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(LTCOMPILE) -c -o $@ $<

PRETTY_MAKE=${top_srcdir}/config/pretty-make
PRETTY_COMPILE=${PRETTY_MAKE} Compiling

endif

COMPILE = ${PRETTY_COMPILE} \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = ${PRETTY_COMPILE}  \
	$(LIBTOOL) --quiet --tag=CC --mode=compile \
	$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)



