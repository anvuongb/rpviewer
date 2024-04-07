#!/bin/bash

set -xe

CFLAGS="-Wall -Werror -I. `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib`"

g++ -std=c++17 $CFLAGS -o rpviewer main.cpp $LIBS
