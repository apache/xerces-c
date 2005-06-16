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

noinst_PROGRAMS =					${sampleprogs}

LDADD =								../obj/libxerces.la
AM_CPPFLAGS =						-I${top_srcdir}/src

sampleprogs =

sampleprogs +=						CreateDOMDocument
CreateDOMDocument_SOURCES =			src/CreateDOMDocument/CreateDOMDocument.cpp

sampleprogs +=						DOMCount
DOMCount_SOURCES =					src/DOMCount/DOMCount.cpp

sampleprogs +=						DOMPrint
DOMPrint_SOURCES =					src/DOMPrint/DOMPrint.cpp \
									src/DOMPrint/DOMPrintErrorHandler.cpp \
									src/DOMPrint/DOMPrintFilter.cpp \
									src/DOMPrint/DOMTreeErrorReporter.cpp

sampleprogs +=						EnumVal
EnumVal_SOURCES =					src/EnumVal/EnumVal.cpp

sampleprogs +=						MemParse
MemParse_SOURCES =					src/MemParse/MemParse.cpp \
									src/MemParse/MemParseHandlers.cpp

sampleprogs +=						PParse
PParse_SOURCES =					src/PParse/PParse.cpp \
									src/PParse/PParseHandlers.cpp

sampleprogs +=						PSVIWriter
PSVIWriter_SOURCES =				src/PSVIWriter/PSVIUni.cpp \
									src/PSVIWriter/PSVIWriter.cpp \
									src/PSVIWriter/PSVIWriterHandlers.cpp

sampleprogs +=						Redirect
Redirect_SOURCES =					src/Redirect/Redirect.cpp \
									src/Redirect/RedirectHandlers.cpp

sampleprogs +=						SAX2Count
SAX2Count_SOURCES =					src/SAX2Count/SAX2Count.cpp \
									src/SAX2Count/SAX2CountHandlers.cpp

sampleprogs +=						SAX2Print
SAX2Print_SOURCES =					src/SAX2Print/SAX2FilterHandlers.cpp \
									src/SAX2Print/SAX2Print.cpp \
									src/SAX2Print/SAX2PrintHandlers.cpp

sampleprogs +=						SAXCount
SAXCount_SOURCES =					src/SAXCount/SAXCount.cpp \
									src/SAXCount/SAXCountHandlers.cpp

sampleprogs +=						SAXPrint
SAXPrint_SOURCES =					src/SAXPrint/SAXPrint.cpp \
									src/SAXPrint/SAXPrintHandlers.cpp

sampleprogs +=						SCMPrint
SCMPrint_SOURCES =					src/SCMPrint/SCMPrint.cpp

sampleprogs +=						SEnumVal
SEnumVal_SOURCES =					src/SEnumVal/SEnumVal.cpp

sampleprogs +=						StdInParse
StdInParse_SOURCES =				src/StdInParse/StdInParse.cpp \
									src/StdInParse/StdInParseHandlers.cpp
									
									
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



