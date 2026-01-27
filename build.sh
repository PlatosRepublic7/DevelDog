#!/bin/bash

ARG=$1
run_app_toggle="0"

if [[ "$ARG" != "APP" ]]; then
    build_app_flag="BUILD_APP=OFF"
else
    build_app_flag="BUILD_APP=ON"
    run_app_toggle="1"
fi

echo "Removing old build files..."
rm -rf build release

# We should build using Debug for the time begin, before moving
# on to RelWithDebInfo and then finally Release
echo "Configuring build..."
mkdir build release
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_SHARED_LIBS=ON \
    -D${build_app_flag} \
    -DCMAKE_INSTALL_PREFIX=./release

if [ $? -ne 0 ]; then
    echo "Build configuration failed. Exiting..."
    exit 1
fi

echo "Building..."
cmake --build build
if [ $? -ne 0 ]; then
    echo "Build failed. Exiting..."
    exit 1
fi

echo "Installing..."
cmake --install build

echo "Testing with $(nproc) threads..."
ctest --test-dir build -j $(nproc) --output-on-failure

if [[ "$run_app_toggle" == "1" ]]; then
    echo "Running Application..."
    ./release/bin/devel_dog
fi
