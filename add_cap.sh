#!/usr/bin/bash

UNIT_TESTS=`find plugins -name '*unittest-*' -type f`
for ut in $UNIT_TESTS
do
    CAPS=`getcap $ut`
    if [ -z "$CAPS" ]
    then
        sudo setcap cap_net_admin,cap_net_raw=eip $ut
    fi
done