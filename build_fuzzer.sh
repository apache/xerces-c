#!/bin/bash
set -e
rm -rf build
mkdir build
cd build
CC=clang CXX=clang++  CXXFLAGS="-std=c++14" cmake .. -DXERCES_BUILD_FUZZERS=1 -Wfatal-errors
CC=clang CXX=clang++  CXXFLAGS="-std=c++14" make -j8

