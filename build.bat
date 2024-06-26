@echo off
if exist .\build rmdir .\build /q /s
if exist .\solution rmdir .\solution /q /s

conan install conanfile.txt --build=missing
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=".\\build\\generators\\conan_toolchain.cmake"
cd ..
cmake --build build --config Release
cmake --install ./build