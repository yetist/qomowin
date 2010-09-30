#!/bin/sh
# C=国家,ST=省份/地区,L=城市,O=组织,OU=容器,CN=对像,E=联系邮箱
if [ ! -f mycert.pem ]
then
	openssl req -x509 -nodes -days 365 \
		-subj '/C=CN/ST=Beijing/L=Beijing/O=Linux Ren Open Source Community./CN=www.linux-ren.org/E=yetist@gmail.com' \
		-newkey rsa:1024 -keyout mycert.pem -out mycert.pem
fi 
if [ ! -f mycert.pfx ]
then
	openssl pkcs12 -export -out mycert.pfx -in mycert.pem -name "Yetist Certificate"
fi

# Example Conversion
# PVK
# export certificate and passphrase-less key
if [ ! -f mykey.pvk ]
then
	openssl pkcs12 -in mycert.pfx -nocerts -nodes -out rsa.pem
	openssl rsa -in rsa.pem -outform PVK -pvk-strong -out mykey.pvk
fi

# SPC
if [ ! -f cert.spc ]
then
	openssl pkcs12 -in mycert.pfx -nokeys -nodes -out cert.pem
	openssl crl2pkcs7 -nocrl -certfile cert.pem -outform DER -out cert.spc
fi
