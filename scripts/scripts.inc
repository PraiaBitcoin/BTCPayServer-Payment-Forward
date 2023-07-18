#! /bin/bash

readonly TK_PARAM=0
readonly TK_ENGINE=1
readonly TK_URL=2
readonly TK_USER=3
readonly TK_PASS=4
readonly TK_LNPAY=5
readonly TK_LOGGED=6
readonly TK_ACCESSTOKEN=7

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
   
   token_url=$(dirname "$token_url/.")

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

   if [[ "$token_engine" == "lndhub" ]]
   then
     #if it is a lnbits extension, lets use as lnbits itself
     local txt="${token_url%/lndhub/ext}"
     if [[ "$txt" != "$token_url" ]]
     then
       token_lnpay=""
       token_url="$txt"
       token_engine="lnbits"
     fi
   fi

   token_criada[$TK_PARAM]="$token_param"
   token_criada[$TK_ENGINE]="$token_engine"
   token_criada[$TK_URL]="$token_url"
   token_criada[$TK_USER]="$token_user"
   token_criada[$TK_PASS]="$token_pass"
   token_criada[$TK_LNPAY]="$token_lnpay"
   token_criada[$TK_LOGGED]="0"
   token_criada[$TK_ACCESSTOKEN]=""

   echo  "==============================="
   echo  "Engine: ${token_criada[$TK_ENGINE]}"				
   echo  "User..: ${token_criada[$TK_USER]}"
   debug "Pass..: ${token_criada[$TK_PASS]}"
   echo  "URL...: ${token_criada[$TK_URL]}"
   debug "Token.: ${token_criada[$TK_PARAM]}"
   echo  "LNPay.: ${token_criada[$TK_LNPAY]}"
   echo  "Token2: ${token_criada[$TK_ACCESSTOKEN]}"
   echo  "==============================="

}


parsetoken_payer () {
   parsetoken "$1"
   
   token_payer=("${token_criada[@]}")
   
   echo  "==============================="
   echo  "Payer Engine: ${token_payer[$TK_ENGINE]}"				
   echo  "Payer User..: ${token_payer[$TK_USER]}"
   debug "Payer Pass..: ${token_payer[$TK_PASS]}"
   echo  "Payer URL...: ${token_payer[$TK_URL]}"
   debug "Payer Token.: ${token_payer[$TK_PARAM]}"
   echo  "Payer LNPay.: ${token_payer[$TK_LNPAY]}"
   echo  "Payer Token2: ${token_payer[$TK_ACCESSTOKEN]}"
   echo  "==============================="
    
}


auth_LNDHUB () {
  local token="$1"
  if [ -z "$token" ]
  then
    echo "No token defined"
    return 1
  fi
  
  local url="$(eval ""echo \${$token[$TK_URL]}"")"
  local user="$(eval ""echo \${$token[$TK_USER]}"")"
  local pass="$(eval ""echo \${$token[$TK_PASS]}"")"
  local engine="$(eval ""echo \${$token[$TK_ENGINE]}"")"
  local logado="$(eval ""echo \${$token[$TK_LOGGED]}"")"
  if [[ $logado -gt 0 ]]
  then
    return 0
  fi
  
  local data="{\"login\":\"$user\",\"password\":\"$pass\"}"
  local ret=$(curl -s -X POST "$url/auth?type=auth" -d "$data" -H "Content-type: application/json")
  local status=$?
  
  if [[ "$status" -gt 0 ]]
  then
    return $status
  fi
  
  local tk=$(echo "$ret"|jq -r .access_token)
  if [[ -n "$tk" ]]
  then
    eval "$token[$TK_LOGGED]=1"
    eval "$token[$TK_ACCESSTOKEN]=""$tk"""
    
#    debug  "Payer Logger: ${token_payer[$TK_LOGGED]}"
#    debug  "Payer Token2: ${token_payer[$TK_ACCESSTOKEN]}"    
    return 0
  fi
  return 1


}

getBalance () {
   local token="$1"
   if [[ -z "$token" ]]
   then
     local token="token_payer"
   fi  
   local getSaldoResult=""  
   local status=1
   local url="$(eval ""echo \${$token[$TK_URL]}"")"
   local pass="$(eval ""echo \${$token[$TK_PASS]}"")"
   local engine="$(eval ""echo \${$token[$TK_ENGINE]}"")"
   
   
   case "$engine" in
     lnbits )
       local ret=$(curl -s -X GET "$url/api/v1/wallet" -H "X-Api-Key: $pass")
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
       
     lndhub )
       auth_LNDHUB "$token"
       local acesstoken="$(eval ""echo \${$token[$TK_ACCESSTOKEN]}"")"
       local ret=$(curl -s -X GET "$url/balance" -H "authorization: Bearer $acesstoken")

       local status=$?
       if [[ $status -gt 0 ]]
       then
         local getSaldoResult="$ret"
       else
         local getSaldoResult=$(echo $ret|jq -r .BTC.AvailableBalance)
       fi
       ;;       
      
    * )     
       local getSaldoResult="Engine [$engine] não implementada"
       ;;
   esac
   
   
   echo $getSaldoResult
   
   return $status
   
}

pay_invoice () {
   local invoice="$1"
   local comment="$2"
   local payer="$3"
   if [[ -z $payer ]]
   then
     local payer='token_payer'
   fi
   local url="$(eval ""echo \${$payer[$TK_URL]}"")"
   local user="$(eval ""echo \${$payer[$TK_USER]}"")"
   local pass="$(eval ""echo \${$payer[$TK_PASS]}"")"
   local engine="$(eval ""echo \${$payer[$TK_ENGINE]}"")"

   status_pay=1
   if [ -z "$invoice" ]
   then
      echo "There is no invoice to pay"
      return $status_pay
   fi
   
   case "$engine" in
     lncli )
       ret_pay=$(lncli --macaroonpath=/mnt/hdd/mynode/lnd/data/chain/bitcoin/mainnet/admin.macaroon --rpcserver=localhost:10009 --lnddir=/mnt/hdd/mynode/lnd payinvoice $maxfee -f --allow_self_payment $channel "$invoice" 2>&1)
     
       status_pay=$?
     
       ;;
     
     lnbits )
     
       ret_pay=$(curl -s -X POST "$url/api/v1/payments" -d "{\"out\": true, \"bolt11\": \"$invoice\", \"extra\": {} }" -H "X-Api-Key: $pass" -H "Content-type: application/json")
       status_pay=$?

       if [ $status_pay -eq 0 ]
       then
         local pay_hash=$(echo $ret_pay|jq -r .payment_hash)
	 if [ "$pay_hash" == "null" ]
	 then
	   status_pay=1
	 fi       
       fi     
       ;;       
       
     lndhub )
       auth_LNDHUB "$payer"
              
       local acesstoken="$(eval ""echo \${$payer[$TK_ACCESSTOKEN]}"")"
       local data="{\"invoice\":\"$invoice\"}"
       
       local ret_pay=$(curl -s -X POST "$url/payinvoice" -d "$data" -H "Content-type: application/json" -H "authorization: Bearer $acesstoken")
       status_pay=$?       
       if [ $status_pay -eq 0 ]
       then
         local pay_hash=$(echo $ret_pay|jq -r .payment_hash)
	 if [[ "$pay_hash" == "null" || -z "$pay_hash" ]]
	 then
	   status_pay=1
	   echo "******************************************"
	 fi       
	 
         echo $ret
	 #local pay_hash=$(echo $ret_pay|jq -r .payment_hash)
	 #if [ "$pay_hash" == "null" ]
	 #then
	 #status_pay=1
	 #fi
       fi							   
       ;;
			  
     * )
       ret_pay="$engine: Engine não implementada."
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
   elif echo "$ret_pay" | grep "required" &> /dev/null
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
