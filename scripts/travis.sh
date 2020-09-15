#!/usr/bin/env bash

mkdir -p ${HOME}/.cache
DEPS_DIR="${HOME}/.cache/deps"
mkdir -p ${DEPS_DIR} && cd ${DEPS_DIR}

if [ ! -d "cmake-install" ]
then
    wget --no-check-certificate https://cmake.org/files/v3.17/cmake-3.17.4-Linux-x86_64.tar.gz
    echo "126cc8356907913787d4ff35237ae1854c09b927a35dbe5270dd571ae224bdd3 *cmake-3.17.4-Linux-x86_64.tar.gz" > cmake_sha256.txt
    sha256sum -c cmake_sha256.txt
    tar -xvf cmake-3.17.4-Linux-x86_64.tar.gz > /dev/null
    mv cmake-3.17.4-Linux-x86_64 cmake-install
fi

PATH=${DEPS_DIR}/cmake-install:${DEPS_DIR}/cmake-install/bin:$PATH
cd ${TRAVIS_BUILD_DIR}