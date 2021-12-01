#!/usr/bin/env bash

set -uex

SCRIPT_PATH=$(cd $(dirname $0); pwd)
PROJECT_ROOT_PATH="${SCRIPT_PATH}/../../"

# Build
mkdir -p ${PROJECT_ROOT_PATH}build && cd ${PROJECT_ROOT_PATH}build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Run
websockify -D --web=/usr/share/novnc/ 80 localhost:5901
echo "=> http://localhost:6080/vnc.html"
qemu-system-i386 -drive file=${PROJECT_ROOT_PATH}build/bin/meimei.iso,format=raw,if=floppy -vnc :1
