#!/bin/bash

set -xe

CFLAGS="-Wall -Werror -I/usr/local/include -L/usr/local/lib `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib`"

clang++ -std=c++17 $CFLAGS -o rpviewer main.cpp $LIBS
