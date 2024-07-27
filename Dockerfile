FROM debian:latest

RUN apt-get update && apt-get -y upgrade

RUN apt-get install -y ninja-build cmake wget git lsb-release wget software-properties-common gnupg

RUN wget https://apt.llvm.org/llvm.sh
RUN chmod +x llvm.sh
RUN ./llvm.sh 18 all

ADD . /bizu
WORKDIR /bizu

RUN cmake --preset debug -DCMAKE_CXX_COMPILER=clang++-18 -DCMAKE_C_COMPILER=clang-18
RUN cmake --build build

EXPOSE 8000 8000

ENTRYPOINT ["./build/debug/bizu"]
