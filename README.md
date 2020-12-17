# REST server

This repository contains simple REST server written in C++ and Node.js and Dockerfile that was used to create docker image. \
Image is of Ubuntu 18.04 version which has installed all modules needed to run the servers. \

# Downloading image
Image can be downloaded from the repository: \
https://hub.docker.com/repository/docker/664296700/rest-server \
using command: docker pull 664296700/rest-server:first-try \

# Running server
Servers were written in such way that first you need to run docker image in interactive mode using command: 

sudo docker run -it 664296700/rest-server:first-try 

we can exit image at any moment using: exit 

then we check the ip of the image using: ifconfig 
and we finally can run server 

1. C++ \
To run server we use command: \

./server ip_address \

e.g. if using ifconfig we saw ip 172.17.0.2 we write \
./server 172.17.0.2 \

2. Node.js \
To run server we use command: \

nodejs indeks.js ip_address \

e.g. if using ifconfig we saw ip 172.17.0.2 we write \
nodejs index.js 172.17.0.2 \

3. Sending request
Servers were tested using command curl. for examples in points 1 and 2: \
3.1. curl -v -X POST -H "Content-Type: application/json" -d @test_file.json 172.17.0.2:9000 \
3.2. curl -v -X POST -H "Content-Type: application/json" -d @test_file.json 172.17.0.2:9100 \

WARNING
-------
It wasn't tested if the servers run on the same image at the same time \

# Building servers and image

1. C++ \

