#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
NORM="\e[0m"

CC="clang"
BUILD_MODE="-DDEBUG_BUILD"
INC="-I./external/sr -I./external/minilua -I./external/glfw-ez/glfw/include/GLFW -I./src -I./external -I./external/mtar/src"
CFLAGS="-pipe -O0 -Werror -Wpedantic -Wall -std=c11 ${INC} ${BUILD_MODE}"
LIBFLAGS="-Ofast -mavx -maes -march=x86-64 -msse4.1"
LIBS="-lGL -ldl -lX11 -lXi -lm"
OUT="core.out"
SRC="src"
BIN="bin"

mkdir -p ${BIN}

function build_info () {
  echo -e "${YELLOW}[BUILD]${NORM} $1"
}

if [ "$1" = "clean" ]; then
  echo -e "${YELLOW}[CLEAN]${NORM} Removing binaries..."
  rm -rf bin
  echo -e "${YELLOW}[CLEAN]${GREEN} Done.${NORM}"
  exit 0
fi

if [ "$1" = "release" ]; then
  build_info "Building in release mode..."
  BUILD_MODE="-DRELEASE_BUILD"
  CFLAGS="-pipe -Ofast -mavx -maes -march=x86-64 -msse4.1 -Werror -Wpedantic -Wall -std=c11 ${INC} ${BUILD_MODE}"
fi

if ! $(test -f ${BIN}/external.o); then
  build_info "Building external.c"
  if ! $(${CC} ${LIBFLAGS} -w -Wno-error -c ./${SRC}/core/external.c -o ${BIN}/external.o); then
    echo -e "${RED}[ERROR]${NORM} Couldn't build the external stuff."
    exit 1
  fi
fi

build_info "Building the thing..."
if ! $(${CC} ${CFLAGS} -c ${SRC}/core/main.c -o ${BIN}/main.o); then
  echo -e "${RED}[ERROR]${NORM} Couldn't build the thing."
  exit 1
fi

build_info "Linking the thing..."
if ! $(${CC} ${BIN}/*.o ${LIBS} -o ${BIN}/${OUT} -Wl,-O3 -flto -mavx -maes -flto -march=x86-64 -msse4.1 -fprofile-generate -fprofile-use); then
  echo -e "${RED}[ERROR]${NORM} Couldn't link the thing."
  exit 1
fi

build_info "Stripping the thing..."
strip ${BIN}/${OUT}
build_info "${GREEN}Done.${NORM}"

