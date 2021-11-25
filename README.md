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

Create the <a href="https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/etc/nginx/sites.enabled/php.conf">php.conf</a> at /etc/nginx/sites.enabled/

and Restart Nginx

Create the file <a href="https://github.com/BitcoinBeachBR/LNBits-Payment-Forward/blob/main/fs/var/www/html/log.php">log.php</a> at /var/www/html


Run the command to create the folders
sudo su
install -o www-data -d /mnt/hdd/BTCPAYSERVER/
