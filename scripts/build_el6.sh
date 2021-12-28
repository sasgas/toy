#!/bin/bash -e
set -x #echo on
DOCKER_IMAGE_NAME=centos6
BUILD_DIR=build_el6
trap "sudo chown -R $(whoami):$(whoami) ${BUILD_DIR}" EXIT
sudo docker run --rm -v $(pwd):$(pwd) -w $(pwd) --name "${PWD##*/}"_${BUILD_DIR} \
    --env BUILD_DIR=${BUILD_DIR} \
    --env CCACHE_DIR=$(pwd)/${BUILD_DIR}/.ccache \
    ghcr.io/castisdev/${DOCKER_IMAGE_NAME}:1.11 \
    ./scripts/detail/${BUILD_DIR}.sh \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold -L/usr/local/lib -L/usr/local/lib64" \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DOPENSSL_ROOT_DIR=/usr/local/openssl \
    -DOPENSSL_USE_STATIC_LIBS=TRUE \
    $@
