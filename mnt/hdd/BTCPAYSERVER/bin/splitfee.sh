#! /bin/bash

############################################################################    
#
# This script splits fees to several wallets, as part of donations program #
#
############################################################################    

#set -x

set -o noglob

SATSPERBTC=100000000

uniqueidfile () {   
   local tmp
   tmp=$((date "+%Y%m%d-%H%M";echo -n \_;dd if=/dev/hwrng count=20 bs=1 2> /dev/null) |xxd -p -c 80)
   echo $tmp

}

uniqueid () {   
   local tmp
   local _id
   local ong
   ong=$1   
   _id=$2
   tmp=$((echo -n "$ong-";echo -n "$_id" )|base64 -w 200)
   echo -n "$tmp"

}


roundsats () {
      local tmp
      tmp=$1
      #printf "%.0f" $(echo "($tmp/$SATSPERBTC)*SATSPERBTC"|bc -l)
      tmp=$(echo "($tmp/$SATSPERBTC)"|bc -l)
      tmp=$(echo "($tmp*$SATSPERBTC)"|bc -l)
      printf "%.0f" $tmp

}

sats2btc () {
      local tmp
      tmp=$1
      tmp=$(echo "($tmp/$SATSPERBTC)"|bc -l)
      printf "%.8f" $tmp

}


BASE=$(dirname $0)

DONATIONSDIR=/mnt/hdd/BTCPAYSERVER/donations

install -d $DONATIONSDIR


fee=$1
idSource=$2



echo "Fee: $fee, idSource=$idSource"

if [ -z "$fee" ]
then
    echo We need a valid fee value
    exit 1
fi

if [ -z "$idSource" ]
then
    echo We need a valid idSource value
    exit 1
fi


fee=$(roundsats $fee)

if [ $fee -lt 1 ]
then
    echo We need a fee greater than 0
    exit 1
fi

echo "Spliting Fee: $fee sats ($(sats2btc $fee) BTC)"

echo ok

echo $(sats2btc $fee) BTC

echo $BASE

tuples=$($BASE/psql_query "select * from charity order by id")

index=0
while true  
do
	echo '============================================'
	id=$(echo $tuples|jq -r .[$index].id)
	rate=$(echo $tuples|jq -r .[$index].rate)
	url=$(echo $tuples|jq -r .[$index].url)

        ( [ "$id" = "null" ] || [ "$id" = "" ] ) && break
        echo "ID=$id"

        [ "$rate" = "null" ] && continue

	echo "Rate=$rate"
        share=$(roundsats $(echo "$fee*$rate"|bc -l ))
	echo "Share=$share"
        
        
        uid=$(uniqueidfile)
        idDonation=$(uniqueid $id $idSource)

        file=$DONATIONSDIR/$uid

        js=$(jq -c --null-input \
                  --argjson amt $share \
                  --arg id $id \
                  --arg source $idSource \
		  --arg invoiceId "$idDonation" \
                  '{ "type": "donation", "paymentMethod": "BTC-LightningNetwork", "source": $source, "payment": { "value": $amt }, "ong": $id, "invoiceId": $invoiceId }')


        echo "$js" > $file.json
        echo "FILE=$uid.json" > $file.control 
        echo "TOKEN=$(echo -n "$url"|base64 -w 200)" >> $file.control 

	let index++

done


