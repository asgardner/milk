[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

# CHAI

CHAI is a derivative of MILK, much like CACAO, COFFEE, and Atmospheres. It's forked here to support adding modules written in C++ and to enable changes to the CMake build process.

## Download

	git clone --recurse-submodules -b dev https://github.com/asgardner/milk.git chai

## Compile

	mkdir build-username
	cd build-username
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
	sudo make install
