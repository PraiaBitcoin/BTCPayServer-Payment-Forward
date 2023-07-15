FROM debian:bullseye-slim

WORKDIR /usr/src/app

RUN apt-get update && apt-get install -y \
    jed \
    jq \
    python3.9 \
    curl \
    wget \
    pip
    
RUN python3.9 -m pip install lnurl
    
COPY ./faz.sh ./    

COPY mnt/hdd/BTCPAYSERVER/bin/csv2invoice ./
COPY mnt/hdd/BTCPAYSERVER/bin/lnurldecode ./

CMD bash ./faz.sh

    
	    