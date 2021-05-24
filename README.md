# C8

![Space Invaders](https://cgmathproghome.files.wordpress.com/2021/05/d.gif?w=638&zoom=2)

CHIP-8 is an old VM from the 1970-s. This is an implementation of CHIP-8 VM for modern systems. It is not 100% accurate and made for my pleasure. You can use it for reference (or your course work).


## Build

You have to install [CMake](https://cmake.org/) to build this project.  
If you are using Conan, you also need [python](https://www.python.org/) and [pip](https://pip.pypa.io/en/stable/installing/).  
On Windows you need to install Visual Studio.  
On MacOS you need to install XCode.  

### Conan (recommended)

On Windows, Ubuntu and MacOS I would recommend using Conan.  
It might not work on other OS/distro.

#### Setup

[Conan](https://conan.io/) is a C/C++ package manager. It automatially resolves dependencies and downloads them.  
You can install and set up conan using pip.  
We are also adding [bincrafters remote URL](https://bincrafters.github.io/2020/04/19/infrastructure-changes-and-required-actions/). 

```bash
pip install conan --user
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
conan config set general.revisions_enabled=1
```

 
Windows users: don't forget to add [pip script folder](https://www.onlinetutorialspoint.com/python/where-can-i-find-python-pip-in-windows.html)
to your PATH environment variable (if you haven't already).

#### Clone and Build:

```bash
git clone https://github.com/plukraine/c8
cd c8
mkdir build

pushd build
conan install .. --build=missing
popd

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DUSE_CONAN=ON
cmake --build build --config Release
```

### The Old-Fashioned Way

Just install libraries youself and let CMake find them.  
Works for Linux and MacOS.  

Ubuntu:
```bash
sudo apt-get install libsdl2-dev libgtest-dev
```

Arch:
```
sudo pacman -Sy sdl2 gtest
```

MacOS:
```
brew install web-eid/gtest/gtest sdl2
```

```bash
git clone https://github.com/plukraine/c8
cd c8
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Release
```

## Run

Download some [ROMs](https://github.com/dmatlack/chip8/tree/master/roms).  
Locate `c8` executable in `c8/build` folder.  
Execute `c8 200 /path/to/the/rom`.

Use `1234 qwer asdf zxcv` keys and experiment with the game.  
And, most importantly, have fun!
