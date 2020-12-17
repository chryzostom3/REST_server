# template used in this image
FROM ubuntu:18.04
# downloading and installing libraries in ubuntu container
RUN apt-get update
RUN apt -y install build-essential
RUN apt-get -y install git
RUN apt-get -y install cmake
RUN apt-get -y install net-tools
# installing libraries used in C++ e.g. cpprestsdk
RUN apt-get -y install g++ git libboost-atomic-dev libboost-thread-dev libboost-system-dev libboost-date-time-dev libboost-regex-dev libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev openssl libssl-dev ninja-build
RUN apt-get -y install libcpprest-dev
# copying package.json 
COPY package*.json ./
# installing modules for node.js
RUN apt -y install nodejs
RUN apt -y install npm -d

COPY . /app

WORKDIR /app

# compiling C++ server
RUN g++ -o server server.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system