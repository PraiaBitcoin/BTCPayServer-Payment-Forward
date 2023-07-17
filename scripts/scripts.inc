#! /bin/bash

readonly TK_PARAM=0
readonly TK_ENGINE=1
readonly TK_USER=3
readonly TK_PASS=4

debug () {
  if [[ "$DEBUG" -gt 0 ]]
  then  
    echo $1
  fi
}

parsetoken () {
   local token_lnpay=
   local token_param=$1
   local token_user=$(echo "$token_param" | grep -o -P '(?<=://).*(?=@)')
   local token_pass=$(echo "$token_user" | grep -o -P '(?<=:).*(?=)')
   local token_user=$(echo "$token_user" | grep -o -P '(?<=).*(?=:)')
   local token_url=$(echo "$token_param" | grep -o -P '(?<=@).*(?=)')

   if [ -z "$token_url" ]
   then
      local token_engine=$token_param
   else 
     local token_engine="${token_param%%://*}"
   fi 
   
   if [ -z "$token_user" ]
   then
     local token_lnpay="S"
     local token_user=$(echo "$token_param" | grep -o -P '(?<=lnpay://).*(?=@)')
   fi
   if [ -z "$token_user" ]
   then
     local token_user=
   fi
   
   
   
   token_criada[$TK_PARAM]="$token_param"
   token_criada[$TK_ENGINE]="$token_engine"
   token_criada[2]="$token_url"
   token_criada[$TK_USER]="$token_user"
   token_criada[$TK_PASS]="$token_pass"
   token_criada[5]="$token_lnpay"
   token_criada[6]="0"
   token_criada[7]=""

   echo  "==============================="
   echo  "Engine: ${token_criada[$TK_ENGINE]}"				
   echo  "User..: ${token_criada[$TK_USER]}"
   debug "Pass..: ${token_criada[$TK_PASS]}"
   echo  "URL...: ${token_criada[2]}"
   debug "Token.: ${token_criada[$TK_PARAM]}"
   echo  "LNPay.: ${token_criada[5]}"
   echo  "Token2: ${token_criada[7]}"
   echo  "==============================="

}


parsetoken_payer () {
   parsetoken $*
   
   token_payer=("${token_criada[@]}")
   
   echo  "==============================="
   echo  "Payer Engine: ${token_payer[$TK_ENGINE]}"				
   echo  "Payer User..: ${token_payer[$TK_USER]}"
   debug "Payer Pass..: ${token_payer[$TK_PASS]}"
   echo  "Payer URL...: ${token_payer[2]}"
   debug "Payer Token.: ${token_payer[$TK_PARAM]}"
   echo  "Payer LNPay.: ${token_payer[5]}"
   echo  "Payer Token2: ${token_payer[7]}"
   echo  "==============================="
    
}

getBalance () {
   local token=("$@")
   local getSaldoResult=""  
   local status=1
   
   case "${token[$TK_ENGINE]}" in
     lnbits )
       local ret=$(curl -s -X GET "${token_payer[2]}/api/v1/wallet" -H "X-Api-Key: ${token_payer[$TK_PASS]}")
       local status=$?
     
       if [ $status -gt 0 ]
       then
         local getSaldoResult="$ret"
       else
         local getSaldoResult=$(echo $ret|jq -r .balance)
	 local getSaldoResult=$(($getSaldoResult / 1000))
       fi
       							 
       ;;
       
     lncli )
       local ret=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd channelbalance 2>&1)
       local status=$?
       if [[ $status -gt 0 ]]
       then
         local getSaldoResult="$ret"      
       else
         local getSaldoResult=$(echo $ret|jq -r .balance)
       fi
     
       ;;   
      
    * )     
       local getSaldoResult="Engine $2 não implementada"
       ;;
   esac
   
   
   echo $getSaldoResult
   
   return $status
   
}


pay_invoice () {
   invoice="$1"
   comment="$2"
   status_pay=1
   if [ -z "$invoice" ]
   then
      echo "There is no invoice to pay"
      return $status_pay
   fi
   
   case "${token_payer[$TK_ENGINE]}" in
     lncli )
       ret_pay=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd payinvoice $maxfee -f --allow_self_payment $channel "$invoice" 2>&1)
     
       status_pay=$?
     
       ;;
     
     lnbits )
     
       ret_pay=$(curl -s -X POST "${token_payer[2]}/api/v1/payments" -d "{\"out\": true, \"bolt11\": \"$invoice\", \"extra\": {} }" -H "X-Api-Key: ${token_payer[$TK_PASS]}" -H "Content-type: application/json")
       status_pay=$?

       if [ $status_pay -eq 0 ]
       then
         pay_hash=$(echo $ret_pay|jq -r .payment_hash)
	 if [ "$pay_hash" == "null" ]
	 then
	   status_pay=1
	 fi       
       fi     
       ;;
     * )
       ret_pay="${token_payer[$TK_ENGINE]}: Engine não implementada."
       ;;
   esac
   
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
   elif echo "$ret_pay" | grep "validation error" &> /dev/null
   then
      echo "$ret_pay"
      status_pay=1	   
   fi

   if [ $status_pay -ne 0 ]
   then
     echo $ret_pay
   fi  
   return $status_pay				   
}
