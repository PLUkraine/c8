mkdir build

pushd build
conan install .. --build=missing || exit /b
popd

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_CONAN=ON || exit /b
cmake --build build --config Release || exit /b
