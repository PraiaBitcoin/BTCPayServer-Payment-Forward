# Lightning-Payment-Forward
The Lightning Payment Forward script is useful when you need receive funds in your node and forward to other users. 

Requirements 
 - Nginx
 - MyNodeBTC
 - BTCPayServer
 - LNBits
 - PHP-FQM

# Instalation Instrucions
If you dont have php installed on your my node install it running the command 


Create the [php.conf](https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/etc/nginx/sites.enabled/php.conf) at */etc/nginx/sites.enabled/*

*apt-get install php-fpm*

and Restart Nginx
*/etc/init.d/nginx restart*

Create the file [log.php](https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/var/www/html/log.php) at */var/www/html*


Run the command to create the folders

*sudo su*
*install -o www-data -d /mnt/hdd/BTCPAYSERVER/*

Test 
*curl --insecure -d "{}" -X POST http://127.0.0.1:44331/log.php?store=123456789*
