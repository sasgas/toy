#!/bin/bash -e
set -x #echo on
source /opt/rh/devtoolset-9/enable
mkdir -p ${BUILD_DIR} && cd ${BUILD_DIR}
cmake --version
cmake .. -GNinja "${@:1:$#-1}"
ninja --version
ninja "${@: -1}"
cd ..
