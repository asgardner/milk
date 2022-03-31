#!/usr/bin/env bash

# chai-src is now a git submodule, so we don't need to do this here.
# MSG="Module 'CHAI'."
# if [ -d "plugins/chai-src" ]; then
#   echo "${MSG} Already installed -> pulling."
#   (cd plugins/chai-src; git pull --ff --quiet)
# else
#   echo "${MSG}"
#   git clone -b main git@gitlab.com:mmtao/chai-src.git plugins/chai-src --quiet
#  fi
# ./fetch_milk_dev.sh

# Setup shared memory space.
echo "Setup shared memory."
if [ -d "/milk/shm" ]
then
  true
else
  echo "tmpfs /milk/shm tmpfs rw,nosuid,nodev" | sudo tee -a /etc/fstab
  sudo mkdir -p /milk/shm
  sudo mount /milk/shm
fi

# Check if this variable is already set.
echo "Setup environment variables."
if [[ -z "${MILK_SHM_DIR}" ]] 
then
  export MILK_SHM_DIR=/milk/shm
  echo "export MILK_SHM_DIR=/milk/shm" >> ~/.bashrc
fi

# Set up config files.
echo "Setup configuration files."
if [ -d "/etc/chai" ]
then
  true
else
  sudo mkdir -p /etc/chai
  sudo chmod -R 0777 /etc/chai
fi

echo "Done."