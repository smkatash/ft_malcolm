FROM debian:bookworm

RUN apt-get update && apt-get install -y lftp \
    net-tools \
    iproute2 \
    iputils-ping

CMD [ "tail", "-f", "/dev/null" ]


