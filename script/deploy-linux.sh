#!/usr/bin/env bash

rm -rf build/*
cd build
cmake ..
make -j8
cd ..

zip pheee-lin.zip bin/linux/* bin/linux/**/*
