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
# $Id: DOM.hpp,v 1.12 2004/09/08 13:55:38 peiyongz Exp $
#

check_PROGRAMS =					${testprogs}

LDADD =								../obj/libxerces.la
AM_CPPFLAGS =						-I${top_srcdir}/src

testprogs =

testprogs +=						DOMTest
DOMTest_SOURCES = 					src/DOM/DOMTest/DTest.cpp \
									src/DOM/DOMTest/DTest.h

testprogs +=						DOMMemTest
DOMMemTest_SOURCES = 				src/DOM/DOMMemTest/DOMMemTest.cpp

testprogs +=						Normalizer
Normalizer_SOURCES = 				src/DOM/Normalizer/Normalizer.cpp \
									src/DOM/Normalizer/Normalizer.hpp
Normalizer_EXTRA =					src/DOM/Normalizer/expectedOutput

testprogs +=						RangeTest
RangeTest_SOURCES = 				src/DOM/RangeTest/RangeTest.cpp

testprogs +=						Traversal
Traversal_SOURCES = 				src/DOM/Traversal/Traversal.cpp

testprogs +=						TypeInfo
TypeInfo_SOURCES =	 				src/DOM/TypeInfo/TypeInfo.cpp \
									src/DOM/TypeInfo/TypeInfo.hpp
TypeInfo_EXTRA =					src/DOM/TypeInfo/data/combined.dtd \
									src/DOM/TypeInfo/data/combined.xml \
									src/DOM/TypeInfo/data/combined.xsd \
									src/DOM/TypeInfo/data/SecondSchema.xsd \
									src/DOM/TypeInfo/data/TypeInfo.dtd \
									src/DOM/TypeInfo/data/TypeInfo.xml \
									src/DOM/TypeInfo/data/TypeInfo.xsd \
									src/DOM/TypeInfo/data/TypeInfoJustDTD.xml \
									src/DOM/TypeInfo/data/TypeInfoNoDTD.xml \
									src/DOM/TypeInfo/data/TypeInfoNoDTD.xsd

testprogs +=						EncodingTest
EncodingTest_SOURCES = 				src/EncodingTest/EncodingTest.cpp

testprogs +=						InitTermTest
InitTermTest_SOURCES = 				src/InitTermTest/InitTermTest.cpp \
									src/InitTermTest/InitTermTest.hpp

# This tests the deprecated dom, which we don't build at the moment
#testprogs +=						MemHandlerTest
#MemHandlerTest_SOURCES = 			src/MemHandlerTest/MemoryMonitor.cpp \
#									src/MemHandlerTest/MemoryMonitor.hpp \
#									src/MemHandlerTest/SimpleHashPtr.cpp \
#									src/MemHandlerTest/SimpleHashPtr.hpp \
#									src/MemHandlerTest/SimpleValueHashTableOf.hpp

testprogs +=						NetAccessorTest
NetAccessorTest_SOURCES = 			src/NetAccessorTest/NetAccessorTest.cpp

# Doesn't compile under gcc4 for some reason
# dcargill says this is obsolete and we can delete it.
#testprogs +=						ParserTest
#ParserTest_SOURCES = 				src/ParserTest/ParserTest.cpp \
#									src/ParserTest/ParserTest.hpp \
#									src/ParserTest/ParserTest_Parser.cpp \
#									src/ParserTest/ParserTest_Parser.hpp

testprogs +=						ThreadTest
ThreadTest_SOURCES = 				src/ThreadTest/ThreadTest.cpp

# Fails to compile under gcc 4 (ambiguous calls to NullPointerException)
# dcargill says this is obsolete and we can delete it.
#testprogs +=						UtilTests
#UtilTests_SOURCES = 				src/UtilTests/CoreTests_BitSet.cpp \
#									src/UtilTests/CoreTests_CountedPointer.cpp \
#									src/UtilTests/CoreTests_RefArray.cpp \
#									src/UtilTests/CoreTests_RefHashTable.cpp \
#									src/UtilTests/CoreTests_RefStack.cpp \
#									src/UtilTests/CoreTests_RefVector.cpp \
#									src/UtilTests/CoreTests_String.cpp \
#									src/UtilTests/CoreTests_Transcoders.cpp \
#									src/UtilTests/CoreTests_URL.cpp \
#									src/UtilTests/CoreTests_ValueArray.cpp \
#									src/UtilTests/CoreTests_ValueStack.cpp \
#									src/UtilTests/CoreTests_ValueVector.cpp \
#									src/UtilTests/CoreTestsMain.cpp \
#									src/UtilTests/CoreTests.hpp

testprogs +=						XSerializerTest
XSerializerTest_SOURCES = 			src/XSerializerTest/XSerializerHandlers.cpp \
									src/XSerializerTest/XSerializerHandlers.hpp \
									src/XSerializerTest/XSerializerTest.cpp \
									src/XSerializerTest/XSerializerTest.hpp

# Fails to compile under gcc4 (values too long for long type)
testprogs +=						XSValueTest
XSValueTest_SOURCES = 				src/XSValueTest/XSValueTest.cpp \
									src/XSValueTest/XSValueTest.hpp

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



