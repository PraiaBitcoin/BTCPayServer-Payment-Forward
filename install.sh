#! /bin/sh

#set -x

apt-get install php-fpm
apt-get install jq
apt-get install bc

DEST=/mnt/hdd/BTCPAYSERVER
EXEDIR=$(dirname $0)$DEST/bin

install -o www-data -d $DEST
install -o www-data -d $DEST/invoices

install -d $DEST/bin
install -d $DEST/conf
touch $DEST/conf/payer.conf

cp $EXEDIR/* $DEST/bin/ -vf

chmod 755 $DEST/bin/*


$DEST/bin/psql_query create

