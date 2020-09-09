#!/bin/bash
set -e
mkdir -p ../build && cd ../build
cmake ../src -DEnable_GUI=0 -DGUI_BUILD_TESTS=0 -DSLIC3R_BUILD_TESTS=0 -DSLIC3R_STATIC=1 -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j4
