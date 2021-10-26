#!/usr/bin/env bash

MSG="Module 'CHAI'."
if [ -d "plugins/chai-src" ]; then
  echo "${MSG} Already installed -> pulling."
  (cd plugins/chai-src; git pull --ff --quiet)
else
  echo "${MSG}"
  git clone -b main git@gitlab.com:mmtao/chai-src.git plugins/chai-src --quiet
fi

./fetch_milk_dev.sh

