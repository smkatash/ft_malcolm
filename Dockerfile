FROM debian:bookworm

RUN apt-get update && \
    apt-get install -y \
    make \
    build-essential \
    valgrind \
    net-tools \
    libpcap-dev \
    iproute2 \
    ca-certificates  \
    iputils-ping

WORKDIR /app