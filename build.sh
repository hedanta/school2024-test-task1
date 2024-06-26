if [ -d build ]; then
  rm -rf build
fi

if [ -d solution ]; then
  rm -rf solution
fi

conan install conanfile.txt --build=missing
cd build
cmake .. --toolchain ./build/generators/conan_toolchain.cmake
cd ..
cmake --build build --config Release
cmake --install ./build