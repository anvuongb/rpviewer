#!/bin/bash

set -xe

CFLAGS="-Wall -Werror -I. `pkg-config --cflags raylib`"
LIBS="`pkg-config --libs raylib`"

g++ -std=c++17 $CFLAGS main.cpp file_explorer.cpp $LIBS -o rpviewer 
