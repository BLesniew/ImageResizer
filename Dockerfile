FROM ubuntu:24.04 AS build_catch2

RUN apt-get update -y && \
    apt-get install -y \
        cmake=3.28.* \
        git=1:2.43.*

RUN git clone https://github.com/catchorg/Catch2.git



FROM ubuntu:24.04 AS build

# maybe use --no-install-recomends
RUN apt-get update -y && \
apt-get install -y \
cmake=3.28.* \
clang=1:18.* \
libopencv-dev=4.6.*

WORKDIR /resizer/Catch2

COPY --from=build_catch2 /Catch2 .

RUN cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
cmake --build build/ --target install 

WORKDIR /resizer

COPY . .

WORKDIR /resizer/build_docker

RUN cmake .. && \
    cmake --build .
