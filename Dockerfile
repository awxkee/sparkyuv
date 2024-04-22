FROM ubuntu:latest

RUN apt-get -y update && apt-get -y install build-essential ninja-build cmake

WORKDIR /usr/sparkyuv

COPY . .

RUN cmake -G "Ninja" .
RUN ninja

EXPOSE $PORT

CMD [ "make" ]
