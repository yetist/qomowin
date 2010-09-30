#!/bin/sh

generate_cert()
{
	# C=国家,ST=省份/地区,L=城市,O=组织,OU=容器,CN=对像,E=联系邮箱
	KEY=$1
	PEM=$2
	openssl req -x509 -nodes -days 365 \
		-subj '/C=CN/ST=Beijing/L=Beijing/O=Linux Ren Open Source Community./CN=www.linux-ren.org/E=yetist@gmail.com' \
		-newkey rsa:1024 -keyout $KEY -out $PEM
}

cert2spc()
{
	CERT=$1
	SPC=$2
	openssl crl2pkcs7 -nocrl -certfile $CERT -outform der -out $SPC
}

key2der()
{
	KEY=$1
	DER=$2
	PASS=$3
	openssl rsa -in $KEY -passin pass:$PASS -outform der -out $DER
}


cert2pfx()
{
	PEM=$1
	PFX=$2
	KEY=$3
	PASS=$4
	openssl pkcs12 -export -inkey $KEY -password pass:$PASS -in $PEM -out $PFX
}

pfx2pvk()
{
	PFX=$1
	PVK=$2
	PASS=$3
	openssl pkcs12 -in $PFX -passin pass:$PASS -nocerts -nodes -out rsa.pem

	openssl rsa -in rsa.pem -passout pass:$PASS -outform pvk -pvk-strong -out $PVK
	rm -f rsa.pem
}

pfx2spc()
{
	PFX=$1
	SPC=$2
	PASS=$3
	openssl pkcs12 -in $PFX -passin pass:$PASS -nokeys -nodes -out cert.pem
	openssl crl2pkcs7 -nocrl -certfile cert.pem -outform der -out $SPC
	rm -f cert.pem
}

echo "call generate_cert"
generate_cert yetist.key yetist.pem

echo "call cert2spc"
cert2spc yetist.pem yetist0.spc

echo "call key2der"
key2der yetist.key yetist.der abc123

echo "call cert2pfx"
cert2pfx yetist.pem yetist.pfx yetist.key abc123

echo "call pfx2pvk"
pfx2pvk yetist.pfx yetist.pvk abc123

echo "call pfx2spc"
pfx2spc yetist.pfx yetist.spc abc123

# ./osslsigncode -spc yetist.spc -key yetist.der -n "ssss" -in qomowin.exe -out yetist.exe
