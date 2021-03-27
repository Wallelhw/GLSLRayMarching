REM git clone --recursive https://github.com/MacgyverLin/GLSLRayMarching.git
REM GLSLRayMarching
@echo off
cd 3rdparty

cd rttr
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cd ..
cd ..

cd zlib
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cd ..
cd ..

cd rapidjson
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cd ..
cd ..