#!/bin/bash -e
set -x #echo on
DOCKER_IMAGE_NAME=rockylinux8
BUILD_DIR=build_el8_valgrind
trap "sudo chown -R $(whoami):$(whoami) ${BUILD_DIR}" EXIT
sudo docker run --rm -v $(pwd):$(pwd) -w $(pwd) --name "${PWD##*/}"_${BUILD_DIR} \
    --env Boost_DIR=/usr/local/boost_1_78_0_valgrind \
    --env BUILD_DIR=${BUILD_DIR} \
    --env CCACHE_DIR=$(pwd)/${BUILD_DIR}/.ccache \
    ghcr.io/castisdev/${DOCKER_IMAGE_NAME}:1.11 \
    ./scripts/detail/${BUILD_DIR}.sh \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold -L/usr/local/lib -L/usr/local/lib64" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    $@
