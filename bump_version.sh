#!/usr/bin/bash

sed -i -r 's/VERSION_PATCH [0-9]*[0-9]/&%/g;:a {s/0%/1/g;s/1%/2/g;s/2%/3/g;s/3%/4/g;s/4%/5/g;s/5%/6/g;s/6%/7/g;s/7%/8/g;s/8%/9/g;s/9%/%0/g;t a};s/%/1/g' ../CMakeLists.txt
