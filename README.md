[![Build Status](https://github.com/charlieSewell/ICT397-Game-Engine/workflows/Windows/badge.svg)](https://github.com/charlieSewell/ICT397-Game-Engine/actions/workflows/Windows.yml)
[![Build Status](https://github.com/charlieSewell/ICT397-Game-Engine/workflows/macOS/badge.svg)](https://github.com/charlieSewell/ICT397-Game-Engine/actions/workflows/macOS.yml)
# ICT397-Game-Engine
<p align=center>
  <b> ICT397 Game Engine </b>
</p>

<p align="center">
  <a href="#building">Building</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a>
</p>


## Building
### Tools
* macOS: [xcode][xcode command line tool][brew][brew-dl]
* Windows: [vcpkg][vcpkg-dl], [Visual Studio 2019][VS2019-dl]

### Dependencies
* Linux: `sudo apt-get install cmake libsdl2-dev libsdl2-image-dev
    libsdl2-ttf-dev libsdl2-mixer-dev libglm-dev assimp-utils`
* macOS: `brew install cmake ninja llvm --with-toolchain`
* Windows: `None` 

### GIT LFS
Ensure you have GIT LFS installed.
```
git lfs install
```

### Cloning
```
git clone https://github.com/charlieSewell/ICT397-Game-Engine.git
cd ICT397-Game-Engine
git checkout master
git submodule update --init --recursive --depth 1
```

### Building

#### Linux & macOS:
```
CXX=clang++ cmake -S . -B build -G Ninja -D CMAKE_BUILD_TYPE=Release
```
Change directory to whatever you specified X as.
```
cmake --build . && ./ICT397-Game-Engine
```

#### Windows:
    * Open Visual Studio as Administrator (required to make symbolic links)
    * In Visual Studio, select Open → CMake, select `CMakeLists.txt`
    * From the "Select Startup Item" menu, select `ICT397-Game-Engine.exe`

## License
This project is licensed under the MIT license. Please see the [LICENSE](LICENSE) file
for details
