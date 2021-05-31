#/bin/bash

set -eux

mkdir -p build

pushd build
case "$(uname -s)" in
    Linux)
        conan install .. --build=missing -s compiler.libcxx=libstdc++11
        ;;
    *)
        conan install .. --build=missing -s compiler.libcxx=libstdc++
        ;;
esac
popd

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_CONAN=ON
cmake --build build --config Release
