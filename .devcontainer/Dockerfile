FROM ubuntu:24.04

# Install system dependencies + Curses headers
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    git \
    cmake \
    sudo \
    libncurses5-dev \
    libncursesw5-dev \
    libsdl2-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /root/code

# install gtest
RUN cd /root/code && git clone https://github.com/google/googletest.git

# install folly to folly-deps
RUN cd /root/code && git clone https://github.com/facebook/folly

RUN apt-get update && cd /root/code/folly && python3 -m venv robotvenv && . ./robotvenv/bin/activate \ 
&& apt-get install -y openssl libssl-dev && ./build/fbcode_builder/getdeps.py install-system-deps --recursive \
&& python3 ./build/fbcode_builder/getdeps.py --allow-system-packages --scratch-path /root/code/folly-deps build \ 
&& rm -rf /root/code/folly-deps/build && rm -rf /root/code/folly-deps/source && rm -rf /var/lib/apt/lists/*