[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

# CHAI

CHAI is a derivative of MILK, much like CACAO, COFFEE, and Atmospheres. It's forked here to support adding modules written in C++ and to enable changes to the CMake build process.

## Prerequisites

Some of the libraries that CHAI depends on aren't available through vcpkg. Because of that, you'll need to install them with your system package manager. For Oracle Linux (and other RHEL derivatives), that looks like this:

	dnf install -y python2 python3 python3-devel fftw3-devel zlib-devel net-snmp-devel readline-devel cmake libconfig glib2-devel c-ares-devel qt5-qtbase-devel qt5-qttools-devel qt5-qtmultimedia-devel libpcap libpcap-devel libxml2-devel perl libX11-devel libXft-devel libgcrypt-devel gperf libX11-devel libXft-devel libXext-devel libXext-devel libXrandr-devel libXi-devel libXcursor-devel libXdamage-devel libXinerama-devel libXxf86vm-devel mesa-libEGL-devel 

The rest of the library dependencies are acquired with vcpkg. 

First, setup:

	git clone https://github.com/microsoft/vcpkg
	cd vcpkg
	./bootstrap-vcpkg.sh
	
Next, build.

	./vcpkg install grpc cairo[fontconfig,freetype,gobject] opencv \
		boost-accumulators boost-beast boost-interprocess boost-json \
		boost-random boost-system boost-test boost-thread cfitsio \
		cpp-httplib eigen3 flatbuffers gflags lodepng sqlite3 \
		sqlitecpp yaml-cpp gsl hdf5[szip,threadsafe,tools,utils,zlib]

## Download

	git clone --recurse-submodules -b dev git@github.com:asgardner/milk.git chai
	git clone git@gitlab.com:mmtao/chai-config.git /etc/chai

## Compile

In order to build `chai`, you will need to make sure your build environment is ready. If you're using Oracle Linux 8, [these instructions](https://gitlab.com/-/snippets/2151560) are recommended.

	./setup_chai_dev.sh
	mkdir build-username
	cd build-username
	cmake -DCMAKE_BUILD_TYPE=Release ..
	make
	sudo make install

## A note on ethernet device names.
The `indi` service in `mmtao-main` expects to find the FPGA and housekeeper ethernet devices at `enp216s0f0` and `enp216s0f1`. 
Fortunately, you can rename ethernet devices in Linux. Assuming those interfaces are `eno3` and `eno4` on your server, here is how to do that.

```bash
ifconfig eno3 down
ip link set eno3 name enp216s0f0
mv /etc/sysconfig/network-scripts/ifcfg-eno3 /etc/sysconfig/network-scripts/ifcfg-enp216s0f0
sed -i -e 's/eno3/enp216s0f0/' /etc/sysconfig/network-scripts/ifcfg-enp216s0f0
ifconfig enp216s0f0 up
```

and then the second interface:

```bash
ifconfig eno4 down
ip link set eno4 name enp216s0f1
mv /etc/sysconfig/network-scripts/ifcfg-eno4 /etc/sysconfig/network-scripts/ifcfg-enp216s0f1
sed -i -e 's/eno3/enp216s0f1/' /etc/sysconfig/network-scripts/ifcfg-enp216s0f1
ifconfig enp216s0f1 up
```