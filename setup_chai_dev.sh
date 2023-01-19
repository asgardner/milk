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

# Checkout branches for milk.
cd src/ImageStreamIO && git checkout dev && cd -

# Checkout branches for plugins.
RET_DIR=`pwd`
cd plugins/cacao-addons/milk-zmq/           ; git checkout main   ; cd $RET_DIR
cd plugins/cacao-addons/mxlib/              ; git checkout main   ; cd $RET_DIR
cd plugins/cacao-addons/rtimv/              ; git checkout main   ; cd $RET_DIR
cd plugins/cacao-addons/xrif/               ; git checkout main   ; cd $RET_DIR
cd plugins/chai-src/                        ; git checkout main   ; cd $RET_DIR
cd plugins/chai-src/proto/                  ; git checkout main   ; cd $RET_DIR
cd plugins/milk-extra-src/clustering/       ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/fft/              ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/image_basic/      ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/image_filter/     ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/image_format/     ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/image_gen/        ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/img_reduce/       ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/info/             ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/kdtree/           ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/linARfilterPred/  ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/linopt_imtools/   ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/psf/              ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/statistic/        ; git checkout dev    ; cd $RET_DIR
cd plugins/milk-extra-src/ZernikePolyn/     ; git checkout dev    ; cd $RET_DIR
cd plugins/mmtao-main/mmtao-main/           ; git checkout main   ; cd $RET_DIR

echo "Done."
