# PAYLNURL

PAYLNURL was designed to take some csv files containing names/lnurl pairs and generate invoices from that lnurls, and pay that invoices
That invoices and csv files are placed in commands sub-folder

Example of csv file:
```
name of the wallet 1,LNURL1DP...
name of the wallet 2,LNURL1DP...
```
Example of name of the wallet.invoice file content
```
lnbc150n1pjt847...
```


# About the script
run ./run.sh located in base dir.
That script generates a docker image containing all packages needed for the script to run.
It takes some parameteres to process:
```
-b|--build
   build/rebuild docker image

-a|--amount value
   Amount in millisats to generate invoices

-c|--comment comment
   Comment to the invoice

-p|--pay
   pay generated invoices

-h|--help
   Displays a basic help
```

# How to use
Clone this github repository and run then script, put csv files with name,lnurl pair in it and run the scripts again
```
# git clone https://github.com/PraiaBitcoin/BTCPayServer-Payment-Forward.git /usr/src/paylnurl
# cd /usr/src/paylnurl
# ./run.sh
# vi ./commands/list.csv
   x1,lnurl...
   x2,lnurl...

# ./run.sh -a 10000000 -c "This is a test tip" -p
```
This example creates a example in commans/list.csv, runs script, generates those invoices with 10k sats (10M msats) and pays those invoices
After all are good, the commands/list.csv is renamed to list.csv.done and several files named *.invoice.paid... If there is any file called .invoice, you can run it again, and the script tries to pay thos pending invoices

# File Extensions
.csv - file to generate invoices
.invoice - invoice to pay
.cvs.done - file processed
.invoice.paid - invoice paid



  

