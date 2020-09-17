#!/usr/bin/env bash

mkdir -p "${HOME}/.cache"
DEPS_DIR="${HOME}/.cache/deps"
mkdir -p "${DEPS_DIR}" && cd "${DEPS_DIR}"

if [ ! -d "cmake-install" ]
then
    wget --no-check-certificate https://cmake.org/files/v3.17/cmake-3.17.4-Linux-x86_64.tar.gz
    echo "126cc8356907913787d4ff35237ae1854c09b927a35dbe5270dd571ae224bdd3 *cmake-3.17.4-Linux-x86_64.tar.gz" > cmake_sha256.txt
    sha256sum -c cmake_sha256.txt
    tar -xvf cmake-3.17.4-Linux-x86_64.tar.gz
    mv cmake-3.17.4-Linux-x86_64 cmake-install
    rm cmake-3.17.4-Linux-x86_64.tar.gz
fi

# if [ ! -d "llvm" ]
# then
#     wget --no-check-certificate https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.1/clang+llvm-10.0.1-powerpc64le-linux-rhel-7.4.tar.xz
#     tar -xvf clang+llvm-10.0.1-powerpc64le-linux-rhel-7.4.tar.xz
#     mv clang+llvm-10.0.1-powerpc64le-linux-rhel-7.4 llvm
#     rm clang+llvm-10.0.1-powerpc64le-linux-rhel-7.4.tar.xz
# fi