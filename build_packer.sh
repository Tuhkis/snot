#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
NORM="\e[0m"

CC="clang"
BUILD_MODE="-DDEBUG_BUILD"
INC="-I./src -I./external -I./external/mtar/src"
CFLAGS="-pipe -Ofast -mavx -maes -march=x86-64 -msse4.1 -Werror -Wpedantic -Wall -std=c89 -ansi ${INC} ${BUILD_MODE}"
LIBS=""
OUT="packer.out"
SRC="src/packer"
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

build_info "Building and linking the thing..."
if ! $(${CC} ${CFLAGS} ${SRC}/packer.c -o ${BIN}/${OUT} ${LIBS}); then
  echo -e "${RED}[ERROR]${NORM} Couldn't build or link the thing. See errors."
  exit 1
fi

build_info "Stripping the thing..."
strip ${BIN}/${OUT}
build_info "${GREEN}Done.${NORM}"

