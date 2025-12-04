#!/bin/bash
set -e

echo "Building ARGoS core..."
mkdir -p argos3/build
cd argos3/build
cmake \
	-DARGOS_BUILD_FOR=simulator \
	-DCMAKE_BUILD_TYPE=Release \
	-DARGOS_DOCUMENTATION=OFF \
	-DARGOS_WITH_LUA=OFF \
	../src
make -j4
sudo make install

cd ../../

echo "Updating library cache..."
sudo ldconfig

echo "Build complete!"