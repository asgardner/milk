#!/bin/bash
set -o xtrace

RET_DIR=`pwd`
cd plugins/cacao-addons/milk-zmq/           ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/mxlib/              ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/rtimv/              ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/cacao-addons/xrif/               ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/chai-src/                        ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/chai-src/proto/                  ; git checkout main   ; git pull; cd $RET_DIR
cd plugins/mmtao-main/mmtao-main/           ; git checkout main   ; git pull; cd $RET_DIR
