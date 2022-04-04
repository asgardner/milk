[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

# CHAI

CHAI is a derivative of MILK, much like CACAO, COFFEE, and Atmospheres. It's forked here to support adding modules written in C++ and to enable changes to the CMake build process.

## Prerequisites

Some of the libraries that CHAI depends on aren't available through vcpkg. Because of that, you'll need to install them with your system package manager. For Oracle Linux (and other RHEL derivatives), that looks like this:

	dnf install -y python2 python3 python3-devel fftw3-devel zlib-devel net-snmp-devel readline-devel cmake libconfig glib2-devel c-ares-devel qt5-qtbase-devel qt5-qttools-devel qt5-qtmultimedia-devel libpcap libpcap-devel libxml2-devel perl libX11-devel libXft-devel libgcrypt-devel gperf libX11-devel libXft-devel libXext-devel libXext-devel libXrandr-devel libXi-devel libXcursor-devel libXdamage-devel libXinerama-devel libXxf86vm-devel mesa-libEGL-devel 

The rest of the library dependencies are acquired with vcpkg. Some of these build cleanly every time. Others are (currently) recalcitrant. Here's the recommended method.

First, setup:

	git clone https://github.com/microsoft/vcpkg
	cd vcpkg
	./bootstrap-vcpkg.sh
	mkdir dynamic
	tee > dynamic/x64-linux.cmake << EOF
	set(VCPKG_TARGET_ARCHITECTURE x64)
	set(VCPKG_CRT_LINKAGE dynamic)
	set(VCPKG_LIBRARY_LINKAGE dynamic)
	set(VCPKG_CMAKE_SYSTEM_NAME Linux)
	EOF
	
Next, the libraries that require special attention. As of 2022-04-03, `cairo` doesn't build cleanly. When it fails, vcpkg will tell you where the log file is. It will mention a header that redefines a type. Open this header, delete the 3 (?) redefined types, and try again.

	LD_LIBRARY_PATH=/lib64/:$LD_LIBRARY_PATH:$PWD/installed/x64-linux/lib ./vcpkg install --overlay-triplets=./dynamic grpc cairo[fontconfig,freetype,gobject] opencv --editable

Lastly, the easy libraries that will install cleanly:

	./vcpkg install --overlay-triplets=./dynamic boost-accumulators boost-beast boost-interprocess boost-json boost-random boost-system boost-test boost-thread cfitsio cpp-httplib eigen3 flatbuffers gflags lodepng sqlite3 sqlitecpp yaml-cpp

## Download

	git clone --recurse-submodules -b dev https://github.com/asgardner/milk.git chai

## Compile

In order to build `chai`, you will need to make sure your build environment is ready. If you're using Oracle Linux 8, [these instructions](https://gitlab.com/-/snippets/2151560) are recommended.

	mkdir build-username
	cd build-username
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
	sudo make install
