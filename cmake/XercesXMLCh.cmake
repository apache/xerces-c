# CMake build for xerces-c
#
# Written by Roger Leigh <rleigh@codelibre.net>
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

# check if the Windows API is defined as using wchar_t or unsigned
# short; if it's wchar_t, we need to map XMLCh to be wchar_t (this is
# safe because on Windows wchar_t is used to store UTF-16 codepoints,
# while it is not true on Unix)

include(CheckCXXSourceCompiles)
include(XercesIntTypes)

set(XERCES_XMLCH_T ${XERCES_U16BIT_INT})
set(XERCES_INCLUDE_WCHAR_H 0)
if(WIN32)
  check_cxx_source_compiles("
#include <windows.h>

wchar_t file[] = L\"dummy.file\";

int main() {
  DeleteFileW(file);
  return 0;
}"
    WINDOWS_wchar)

  if(WINDOWS_wchar)
    set(XERCES_XMLCH_T wchar_t)
    set(XERCES_INCLUDE_WCHAR_H 1)
  endif()
endif()
