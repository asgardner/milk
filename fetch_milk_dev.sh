#!/usr/bin/env bash

mkdir -p plugins/milk-extra-src
cd plugins/milk-extra-src

for mod in clustering fft ZernikePolyn cudacomp image_basic image_filter image_format image_gen img_reduce info kdtree linARfilterPred linopt_imtools psf statistic
do
MSG="Module '${mod}'."
if [ -d "${mod}" ]; then
  echo "${MSG} Already installed -> pulling."
  (cd ${mod}; git pull --ff --quiet)
else
  git clone -b dev https://github.com/milk-org/${mod} --quiet
  echo ""
fi
done
