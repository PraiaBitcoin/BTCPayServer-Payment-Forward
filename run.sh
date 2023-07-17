#! /bin/bash

#docker build -t paylnurl .
BASEDIR=$(dirname "$0")

if [[ -f $BASEDIR/scripts/scripts.inc ]]
then
  echo "Incluindo scripts..."
  . $BASEDIR/scripts/scripts.inc
fi

function build() {
   docker build -t paylnurl .   
}

function usage() {
      echo "This script starts or generates a docker image called 'paylnurl', exports 'commands' subdir to it generates and pay invoices"
      echo "Generated files are also in 'commands' sub dir"
      echo "CSV files must have 2 or more columns, containing 'name' and 'lnurl' fields"
      echo 
      echo "Usage: $0 [-b||--build] [[-a||--amount] value]] [-c||--comment] comment] [-p] [-h||--help]"
      echo "   -d,--debug => Print some additional info"
      echo "   -b,--build => build docker image"
      echo "   -a,--amount value => amount in milisats to generate invoices "  
      echo "   -c,--comment comment => Comment "
      echo "   -p,--pay url => Pay invoices"
      echo "   -t,--test  => Test"
      echo "   -h||--help => Shows this help" 
      exit 1
}

DIR=$(pwd)

COMMANDS=$DIR/commands

install -d $COMMANDS

PAY=0
DEBUG=0
TEST=0

while [ $# -gt 0 ]; do
  case "$1" in
    -b | --build )
      build
      shift 1
      ;;
    -t | --test )
      TEST=1
      shift 1
      ;;
    -d | --debug )
      DEBUG=1
      shift 1
      ;;
    -a | --amount )
      AMOUNT="$2"
      shift 2
      ;;
    -c | --comment )
      COMMENT="$2"
      shift 2
      ;;
    -p | --pay )
      PAYER="$2"
      shift 2
      ;;
    -h | --help )
      usage
      ;;
    * )
      echo
      echo "Invalid argument [$1]"
      echo 
      usage
#    * )
#      echo "Invalid argument [$1]"
#      exit 1
      ;;
    esac
 done


if ! docker image inspect paylnurl >/dev/null 2>&1
then
  build  
fi 

if [[ $TEST -gt 0 ]]
then
  if [ ! -z "$PAYER" ]
  then
    parsetoken_payer "$PAYER"
    echo "Balance: $(getSaldo ${token_payer[@]}) sats"
  fi
  exit 0
fi

if [[ "$AMOUNT" == "" || ! "$AMOUNT" =~ ^[0-9]+$ || "$AMOUNT" -lt 1000 ]]
then
  echo "Invalid amount value. use '-a value' to specify it. Remember it is in msats"
  echo
  usage
fi   

docker run --rm --name paylnurl -v $COMMANDS:/usr/src/app/commands -e "AMOUNT=$AMOUNT" -e "COMMENT=$COMMENT" -e "PAY=$PAY" paylnurl 

if [ ! -z  "$PAYER" ]
then  
  echo "Paying invoices"
  
  parsetoken_payer "$PAYER"
    
  for f in ./commands/*.invoice
  do
    if [ ! -f "$f" ]
    then
      echo "Nothing more to pay"
      break
    fi

    invoice=$(cat "$f")
    cmt="$COMMENT for $(basename \"$f\")"
    pay_invoice $invoice $cmt
    
    if [ $status_pay -eq 0 ]
    then
      echo Paid "$invoice on $(basename \"$f\")"
      mv "$f" "$f.paid"
    else 
      echo "Invoice [$invoice] was not paid"
    fi
  done
fi
