#!/bin/bash
set -o xtrace

RET_DIR=`pwd`
cd plugins/cacao-addons/milk-zmq/           ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/mxlib/              ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/rtimv/              ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/xrif/               ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-src/                       ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/chai-src/                        ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/chai-src/proto/                  ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/libs/libtar/                     ; git checkout master ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/clustering/       ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/fft/              ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/image_basic/      ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/image_filter/     ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/image_format/     ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/image_gen/        ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/img_reduce/       ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/info/             ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/kdtree/           ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/linARfilterPred/  ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/linopt_imtools/   ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/psf/              ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/statistic/        ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/milk-extra-src/ZernikePolyn/     ; git checkout dev    ; git pull; cd $RET_DIR
cd plugins/mmtao-main/mmtao-main/           ; git checkout main   ; git pull; cd $RET_DIR
