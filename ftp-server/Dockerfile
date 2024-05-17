FROM debian:bookworm

RUN apt-get update && apt-get install -y vsftpd \
    dumb-init \
    iproute2 \
    iputils-ping 

COPY ./vsftpd.conf /etc/vsftpd.conf
COPY ./script.sh /tmp/

WORKDIR /tmp/
RUN 	chmod +x script.sh
ENTRYPOINT	["/usr/bin/dumb-init"]

CMD			["bash", "/tmp/script.sh"]


