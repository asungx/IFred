FROM ubuntu:22.04

RUN apt-get update && apt-get install -y software-properties-common

RUN add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        curl \
        g++-12 \
        gdb \
        git \
        make \
        ninja-build \
    && update-alternatives --install /usr/bin/c++ c++ /usr/bin/g++-12 99

# Download and install CMake
ARG CMAKE_VERSION="3.23.2"
ARG CMAKE_ARCH="linux-x86_64"
ARG CMAKE_FILENAME="cmake-${CMAKE_VERSION}-${CMAKE_ARCH}"
ARG CMAKE_URL_TARGET="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/${CMAKE_FILENAME}.tar.gz"
RUN curl -sL ${CMAKE_URL_TARGET} -o /tmp/${CMAKE_FILENAME}.tar.gz && \
    tar -zxf /tmp/${CMAKE_FILENAME}.tar.gz --directory /opt && \
    ln -s /opt/${CMAKE_FILENAME}/bin/cmake /bin/cmake

# Install miniconda3
ENV PATH="${PATH}:/root/miniconda3/bin"
ARG CONDA_VERSION="latest"
ARG CONDA_ARCH="Linux-x86_64"
ARG CONDA_FILE="Miniconda3-${CONDA_VERSION}-${CONDA_ARCH}.sh"
ARG CONDA_URL_TARGET="https://repo.anaconda.com/miniconda/${CONDA_FILE}"
RUN curl -sL ${CONDA_URL_TARGET} --output /tmp/${CONDA_FILE} && \
    bash /tmp/${CONDA_FILE} -b

RUN conda install -y --channel=conda-forge xeus=2.4.1 \
    OpenSSL \
    cppzmq \
    ipykernel \
    jupyterlab \
    libuuid \
    nlohmann_json \
    pkg-config \
    xtl \
    zeromq

CMD echo "Container is sleeping..." && sleep infinity
