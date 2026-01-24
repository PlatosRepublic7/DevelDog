#!/bin/bash

echo "Removing old build files..."
rm -rf build release

echo "Configuring build..."
mkdir build release
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo
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

echo "Running Application..."
./release/devel_dog
