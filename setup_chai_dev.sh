#!/usr/bin/env bash

set -o xtrace

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

# Checkout branches.
cd plugins/chai-src && git checkout main && cd -
cd plugins/chai-src/proto && git checkout main && cd -
cd src/ImageStreamIO && git checkout dev && cd -

echo "Done."
