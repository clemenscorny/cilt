#!/usr/bin/env bash

if [ $CXX = "g++" ]; then
    cmake -DENABLE_TESTING_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
else
    cmake -DCMAKE_BUILD_TYPE=Debug ..
fi
