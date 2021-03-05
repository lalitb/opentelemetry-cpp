#!/bin/bash

set -e
export DEBIAN_FRONTEND=noninteractive
setup_type="build_and_install"
if [ -z $1 ]; then
  case "$1" in
     build)
        setup_type="build"
        ;;
     install)
        setup_type="install"
        ;;
     *)
        setup_type="build_and_install"
        ;;
  esac
fi

if ! type cmake > /dev/null; then
    #cmake not installed, exiting
    exit 1
fi
export BUILD_DIR=/tmp/.grpc_build
export INSTALL_DIR=/usr/local/
if [ $setup_type == "build" ] || [ $setup_type == "build_and_install" ];  then
    mkdir -p  ${BUILD_DIR}
    rm -rf ${BUILD_DIR}/*
    pushd $BUILD_DIR
    git clone --recurse-submodules -b v1.34.0 https://github.com/grpc/grpc
    cd grpc
    mkdir -p cmake/build
    pushd cmake/build
    cmake -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        ../..
    make -j $(nproc)
    make install
    popd
    popd
fi
if [ $setup_type == "install" ] || [ $setup_type == "build_and_install" ];  then
    pushd $BUILD_DIR
    make install
    popd
fi
export PATH=${INSTALL_DIR}/bin:$PATH  # ensure to use the installed grpc
