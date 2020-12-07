FROM ubuntu:focal

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update -y &&\
	apt-get install -y --no-install-recommends \
		git \
		g++ \
		gcc-9 \
		gdb \
		valgrind \
		cmake \
		ninja-build \
		libboost-all-dev \
		libglm-dev \
		libyaml-cpp-dev \
		&&\
		rm -rf /var/lib/apt/lists/* &&\
		mkdir -p /data 


WORKDIR /data
COPY start.sh /
CMD ["/start.sh"]
