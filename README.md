[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)

# Milk

milk-core for **milk** package.


Module inclues key frameworks :

- **image streams** : low-latency shared memory streams
- **processinfo** : process management and control
- **function parameter structure (FPS)** : reading/writing function parameters

## Download

	git clone https://github.com/asgardner/milk.git
	cd milk
	./fetch_milk_dev.sh


## Compile

Standard compile:

	mkdir build-username
	cd build-username
	cmake ..
	make
	sudo make install

Compile with Python module (check script help with -h option for details):

    ./compile.sh $PWD/local


## Adding plugins

Compile with cacao plugins:

    ./fetch_cacao_dev.sh
    ./compile.sh $PWD/local

Compile with coffee plugins:

    ./fetch_coffee_dev.sh
    ./compile.sh $PWD/local

Compile with chai plugins:

    ./fetch_chai_dev.sh
	cd build-username
	cmake ..
	make -j8
	sudo make install
