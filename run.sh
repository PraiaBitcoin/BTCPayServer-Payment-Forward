#! /bin/bash

#docker build -t payurl .

DIR=$(pwd)

COMMANDS=$DIR/commands

install -d $COMMANDS

PAY=0

while [ $# -gt 0 ]; do
  case "$1" in
    -a | --amount )
      AMOUNT="$2"
      shift 2
      ;;
    -c | --comment )
      COMMENT="$2"
      shift 2
      ;;
    -p | --pay )
      PAY=1
      shift 1
      ;;
    * )
      echo "Invalid argument [$1]"
      exit 1
      ;;
    esac
 done


docker run --rm --name payurl -v $COMMANDS:/usr/src/app/commands -e "AMOUNT=$AMOUNT" -e "COMMENT=$COMMENT" -e "PAY=$PAY" payurl 

if [ "$PAY" -gt 0 ]
then
  echo "Efetuando pagamentos"
  for f in ./commands/*.invoice
  do

    if [ ! -f "$f" ]
    then
      echo "Nothing more to pay"
      break
    fi

    invoice=$(cat "$f")

    ret=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd payinvoice $maxfee -f --allow_self_payment $channel "$invoice" 2>&1)

    status=$?

    if echo "$ret" | grep "invoice is already paid" &> /dev/null
    then
      echo "Already paid"
      status=0
    fi

    if [ $status -eq 0 ]
    then
      mv "$f" "$f.paid"
    fi
#    echo $ret



  done

fi
