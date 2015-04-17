#!/usr/bin/env bash

if [ $CXX = "g++" ]; then
    make coverage
    coveralls-lcov coverage.info.cleaned
#    coveralls
fi
