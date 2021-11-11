#!/usr/bin/env bash

MSG="Module 'CHAI'."
if [ -d "plugins/chai-src" ]; then
  echo "${MSG} Already installed -> pulling."
  (cd plugins/chai-src; git pull --ff --quiet)
else
  echo "${MSG}"
  git clone -b main git@gitlab.com:mmtao/chai-src.git plugins/chai-src --quiet
 fi

# ./fetch_milk_dev.sh

# Setup shared memory space.

if [ -d "/milk/shm" ]; then
  true;
else
  echo "tmpfs /milk/shm tmpfs rw,nosuid,nodev" | sudo tee -a /etc/fstab
  sudo mkdir -p /milk/shm
  sudo mount /milk/shm
fi

if [[ -z "${MILK_SHM_DIR}" ]]; then
  export MILK_SHM_DIR=/milk/shm
  echo "export MILK_SHM_DIR=/milk/shm" >> ~/.bashrc
fi
