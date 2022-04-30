FROM debian:10 AS builder

RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y gcc mtools make cmake gdb

COPY ./ /usr/local/src/meimei/
RUN mkdir -p /usr/local/src/meimei/tools/generator/build/
WORKDIR /usr/local/src/meimei/tools/generator/build/
RUN cmake .. -DCMAKE_C_COMPILER=gcc -DCMAKE_INSTALL_PREFIX=/usr/local/ -DCMAKE_BUILD_TYPE=Debug && \
    cmake --build . && \
    ctest --output-on-failure && \
    make install
ENV PATH=$PATH:/usr/local/bin/
RUN meimei_generator

RUN mkdir -p /opt/meimei

WORKDIR /opt/meimei


FROM builder AS server

RUN apt-get install -y novnc websockify

RUN apt-get install -y qemu-system-x86 qemu-system-gui qemu-utils ovmf
