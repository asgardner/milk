#!/usr/bin/env bash

set -o xtrace

# Download code

NETSNMP="net-snmp-5.9.1.tar.gz"
NETSNMP_URL="https://gigenet.dl.sourceforge.net/project/net-snmp/net-snmp/5.9.1/net-snmp-5.9.1.tar.gz"
NETSNMP_DIR="net-snmp-5.9.1"

if [ -e $NETSNMP ]
then
    true
else
    wget --quiet $NETSNMP_URL
fi

if [ -e $NETSNMP_DIR ]
then
    true
else
    tar xf $NETSNMP
fi

# Build it.

cd $NETSNMP_DIR
./configure --prefix=/usr/local
sed -i -e 's/NETSNMP_SELECT_TIMEVAL void/NETSNMP_SELECT_TIMEVAL struct timeval/' ./include/net-snmp/net-snmp-config.h
make all -j8
sudo make install
cd ..

# Delete what we no longer need.
rm -rf $NETSNMP_DIR $NETSNMP