## Build

1. Install `conan` on you computer. `pip install conan --user`
1. `cd /path/to/c8`
1. `mkdir build && cd build`
1. `conan install .. --build=missing`
1. `cd .. && cmake -S . -B build` 