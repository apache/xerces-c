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

# Definitions required for building
add_definitions(
  -DHAVE_CONFIG_H=1
)
# Search the project binary dir for config.h
include_directories(
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

macro(add_fuzzer_executable name)
  add_executable(${name} ${ARGN})
  target_link_libraries(${name} xerces-c)
  if (XERCES_BUILD_FUZZERS)
    target_compile_options(${name} PUBLIC -fsanitize=fuzzer)
    target_link_libraries(${name} -fsanitize=fuzzer)
  elseif(XERCES_BUILD_FOR_OSS_FUZZ)
    target_link_libraries(${name} $ENV{LIB_FUZZING_ENGINE})
  endif()
  set_target_properties(${name} PROPERTIES FOLDER "Fuzzers")
endmacro()

add_fuzzer_executable(fuzz_parser_target
  src/xerces_fuzz_common.cpp
  src/parse_target.cpp

)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/external)


ExternalProject_Add(libprotobuf-mutator
  GIT_SHALLOW 1
  GIT_REPOSITORY https://github.com/google/libprotobuf-mutator.git
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release -DLIB_PROTO_MUTATOR_DOWNLOAD_PROTOBUF=ON -DLIB_PROTO_MUTATOR_TESTING=false -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

#set(PROTOC_PATH /usr/local/bin/protoc) #${CMAKE_CURRENT_BINARY_DIR}/libprotobuf-mutator-prefix/src/libprotobuf-mutator-build/external.protobuf/bin/protoc)
set(PROTOC_PATH ${CMAKE_CURRENT_BINARY_DIR}/libprotobuf-mutator-prefix/src/libprotobuf-mutator-build/external.protobuf/bin/protoc)

message(STATUS "### Vincent Debug: ${CMAKE_CURRENT_SOURCE_DIR} ${PROTOC_PATH} ${EXTERNAL_INSTALL_LOCATION} ${CMAKE_CURRENT_BINARY_DIR}/libprotobuf-mutator-prefix/src/libprotobuf-mutator-build/external.protobuf/src/external.protobuf-build/libprotobuf.a")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/genfiles ${CMAKE_CURRENT_BINARY_DIR}/genfiles/xml.pb.cc
  DEPENDS libprotobuf-mutator
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
  COMMAND mkdir   ${CMAKE_CURRENT_BINARY_DIR}/genfiles && ${PROTOC_PATH} xml.proto --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/genfiles
)
add_custom_target(xml_proto_files ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/genfiles)



add_fuzzer_executable(fuzz_parser_target_proto
  src/xerces_fuzz_common.cpp
  src/parse_target_proto.cpp
  src/xmlProtoConverter.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/genfiles/xml.pb.cc
)
add_dependencies(fuzz_parser_target_proto xml_proto_files)
add_dependencies(fuzz_parser_target_proto libprotobuf-mutator)
target_include_directories(fuzz_parser_target_proto PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include
  fuzz_parser_target_proto PUBLIC ${EXTERNAL_INSTALL_LOCATION}/include/libprotobuf-mutator/
  ${EXTERNAL_INSTALL_LOCATION}/../libprotobuf-mutator-prefix/src/libprotobuf-mutator-build/external.protobuf/include
  fuzz_parser_target_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/genfiles
)
#target_include_directories(fuzz_parser_target_proto PUBLIC ${EXTERNAL_INSTALL_LOCATION}/external.protobuf/include)
target_link_libraries(fuzz_parser_target_proto ${EXTERNAL_INSTALL_LOCATION}/lib/libprotobuf-mutator-libfuzzer.a)
target_link_libraries(fuzz_parser_target_proto ${EXTERNAL_INSTALL_LOCATION}/lib/libprotobuf-mutator.a)
target_link_libraries(fuzz_parser_target_proto ${CMAKE_CURRENT_BINARY_DIR}/libprotobuf-mutator-prefix/src/libprotobuf-mutator-build/external.protobuf/src/external.protobuf-build/libprotobuf.a)

