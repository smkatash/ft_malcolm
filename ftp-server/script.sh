#!/bin/bash

useradd --create-home $FTP_USERNAME && echo "$FTP_USERNAME:$FTP_PASSWORD" | chpasswd
mkdir -p /home/$FTP_USERNAME/ftp && chown $FTP_USERNAME:$FTP_USERNAME /home/$FTP_USERNAME/ftp
echo "vsftpd test file" | tee /home/$FTP_USERNAME/ftp/test.txt
echo $FTP_USERNAME | tee -a /etc/vsftpd.userlist
service vsftpd start
tail -f /dev/null

