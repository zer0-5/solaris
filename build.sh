#!/bin/bash
build_dir=build

cmake -S . -B $build_dir

(cd $build_dir && make)
