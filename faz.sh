#! /bin/bash

echo Starting...
for f in ./commands/*.csv 
do
  
  if [ ! -f "$f" ] 
  then
    echo "Done."
    break
  fi
  
  echo "Processing $f."

  cat $f | ./csv2invoice -a "$AMOUNT" -c "$COMMENT"
  mv "$f" "$f.done"
  sleep 1
  
done

exit 0
if [ "$PAY" -gt 0 ]
then
  echo "Paying invoices"
  for f in ./commands/*.invoice
  do
  
    if [ ! -f "$f" ]
    then 
      echo "Nothing more to pay"
      break
    fi
    
    ret=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd payinvoice $maxfee -f --allow_self_payment $channel "$_invoice" 2>&1)
	  
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
    echo $ret
    
    

  done
  
fi
