#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
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

include ../version.incl

src	=						${top_srcdir}/src
lib	=						${top_srcdir}/lib

lib_LTLIBRARIES	= 			libxerces.la
libxerces_la_SOURCES =
nodist_EXTRA_libxerces_la_SOURCES = dummyfile-to-force-cpp-linking.cpp

libxerces_la_LIBADD = 		${src}/libsrc.la ${lib}/libcompat.la
libxerces_la_LDFLAGS =		-release ${PACKAGE_VERSION}

if XERCES_USE_MSGLOADER_ICU
libxerces_la_LDFLAGS +=		-L${top_srcdir}/obj/.libs -lXercesMessages$(VER)
endif

