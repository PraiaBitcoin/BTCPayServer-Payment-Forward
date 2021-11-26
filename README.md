# BtcPayServer Lightning Payment Forward
The Lightning Payment Forward script is useful when you need receive funds in your node and forward to other users. 

At Bitcoin Beach Brazil in Jericoacoara - Ceara, we are running a MyNodeBT with BTCPayServer as *Virtual BTC Hats* for our local artists. When they receive a Lightning donation using theirs PoS connected to our local node, we had to forward the payment manualy. This script was created to automate this process. 

So, when an invoice is setted the BTCPayServer webhook will call a local PHP file, to forward the payment using LNBits wallets, connected to LNDHub, with the end-user using Bluewallet to manage the wallet.

# Brazilian Way
Self-payments are not allowed, then, to not need a second node for the users wallets, and maintain the funds locally, bypassing this restriction, we created an private channel with 0 fee, connected to the node of our DEV. Basicly when the script is called, we will force to use this 0 fee channel to transfer the funds direcly to the end-user wallet back to our local node, but now controled by the user. 

# Requirements
 - MyNodeBTC
 - BTCPayServer
 - LNBits
 - PHP-FQM, BC, JQ 

# Instalation Instrucions
1) Install the needed dependencies
```
apt-get install php-fpm
apt-get install bc
apt-get install jq  
```

2) Create the [php.conf](https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/etc/nginx/sites.enabled/php.conf) at ```/etc/nginx/sites.enabled/```

3) Restart Nginx
```
/etc/init.d/nginx restart
```

4) Create the file [log.php](https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/var/www/html/log.php) at ```/var/www/html```

5) Run the command to create the folders
```
sudo su
install -o www-data -d /mnt/hdd/BTCPAYSERVER/
```

6) Run the command below to test if the files are being created at ```/mnt/hdd/BTCPAYSERVER/invoices```
```
curl --insecure -d "{}" -X POST http://127.0.0.1:44331/log.php?store=123456789
```

7) LNBits configuration. Here we need two wallets, one to make the Payments, and other of the end-user. When the user create a LNBits Wallet, is necessary to activate the extensions, LNURLp and LNHub. After activate, and take note of the wallet URL

   a) The admin will provide the full access 'LNDHUB ADMIN URL' adding it to the ```line X``` of the file [doit](#) located at 
   b) The user will provide only the 'LNDHUB INVOICE URL' at BTCPayServer explained in the next topic


8) BTCPayServer configuration

9) Install guard for loop script - add this line to /etc/rc.local, just before script "exit 0" call
```
( sleep 300; /path/to/your/script/dir/bin/guard_btcpayserver ) &
```
 



