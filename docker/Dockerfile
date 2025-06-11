FROM ubuntu:22.04

RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && \
    sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y \
    autoconf \
    automake \
    autotools-dev \
    curl \
    python3 \
    python3-dev \
    python3-pip \
    python3-tomli \
    libmpc-dev \
    libmpfr-dev \
    libgmp-dev \
    gawk \
    build-essential \
    bison \
    flex \
    texinfo \
    gperf \
    libtool \
    patchutils \
    bc \
    zlib1g \
    zlib1g-dev \
    libexpat-dev \
    ninja-build \
    git \
    cmake \
    libglib2.0-dev \
    libslirp-dev \
    libx11-dev \
    libxft-dev \
    libpng-dev \
    libjpeg-dev \
    help2man \
    perl \
    libfl2 \
    libfl-dev

ENV PATH=$PATH:/opt/riscv32/bin

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/local/lib64:/opt/riscv32/lib

RUN --mount=type=bind,source=deps.tar,target=/opt/deps.tar \
    cd /opt && \
    tar -xf deps.tar --no-same-owner --preserve-permissions && \
    cd verilator && \
    git checkout stable && \
    autoconf && \
    ./configure && \
    make -j $(nproc) && \
    make install && \
    cd .. && \
    rm -rf verilator && \
    cd fltk-1.4.3 && \
    ./configure --enable-debug --enable-shared --enable-threads --enable-xdbe --enable-xft && \
    make -j $(nproc) && \
    make install && \
    cd .. && \
    rm -rf fltk-1.4.3 && \
    mkdir riscv32 && \
    cd riscv-gnu-toolchain && \
    ./configure --prefix=/opt/riscv32 --with-arch=rv32i --with-abi=ilp32 && \
    make -j $(nproc) && \
    cd .. && \
    rm -rf riscv-gnu-toolchain