#/bin/bash

set -eux

mkdir -p build

pushd build
conan install .. --build=missing -s compiler.libcxx=libstdc++11
popd

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_SDL2_MIXER=OFF -DUSE_CONAN=ON
cmake --build build --config Release
