#! /bin/bash

#This script contains basic funcitions to our scripts

parsetoken_payer () {
   payer_lnpay=
   payer_token=$1
   payer_user=$(echo "$payer_token" | grep -o -P '(?<=://).*(?=@)')
   payer_pass=$(echo "$payer_user" | grep -o -P '(?<=:).*(?=)')
   payer_user=$(echo "$payer_user" | grep -o -P '(?<=).*(?=:)')
   payer_host=$(echo "$payer_token" | grep -o -P '(?<=@).*(?=)')
   if [ -z "$payer_host" ]
   then
      payer_engine=$payer_token
   else 
     payer_engine="${payer_token%%://*}"
   fi
   
   if [ -z "payer_$user" ]
   then
     payer_lnpay="S"
     payer_user=$(echo "$payer_token" | grep -o -P '(?<=lnpay://).*(?=@)')
   fi
   if [ -z "$payer_user" ]
   then
     payer_lnpay=
   fi
				
   echo "Engine: $payer_engine"				
   echo "User Payer: $payer_user"
   echo "Pass Payer: $payer_pass"
   echo "Host Payer: $payer_host"
   echo "Token Payer: $payer_token"
   echo "LNPay Payer: $payer_lnpay"
}

pay_invoice () {
   invoice="$1"
   status_pay=1
   if [ -z "$invoice" ]
   then
      echo "There is no invoice to pay"
      return
   fi
   
   if [[ "$payer_engine" == "lncli" ]]
   then
     ret_pay=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd payinvoice $maxfee -f --allow_self_payment $channel "$invoice" 2>&1)

     status_pay=$?
   elif [[ "$payer_engine" == "lnbits" ]]
   then 
     ret_pay=$(curl -s -X POST "$payer_host/api/v1/payments" -d "{\"out\": true, \"bolt11\": \"$invoice\"}" -H "X-Api-Key: $payer_pass" -H "Content-type: application/json")
     status_pay=$?
     status_pay=0
     echo $ret_pay
     pay_hash=$(echo $ret_pay|jq -r .payment_hash)
     if [ -z "$pay_hash" ]
     then
       status_pay=1
       return
     fi  
     
   fi  
   
   if echo "$ret_pay" | grep "invoice is already paid" &> /dev/null
   then
     echo "Already paid"
     status_pay=0
   elif echo "$ret_pay" | grep "invoice already paid" &> /dev/null
   then
     echo "Already paid"
     status_pay=0
   elif echo "$ret_pay" | grep "Bad bech32" &> /dev/null
   then
     echo "$ret_pay"
     status_pay=1
   fi
		       
   
    
				   
}
