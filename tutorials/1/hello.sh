#!/bin/bash

# First, setting environment variables
PATH_NAME=${PWD}
echo "Ready for emscripten"
echo "Execution Path : ${PWD}"

cd ~/Graduation_Project/emsdk
source ./emsdk_env.sh

cd $PATH_NAME

# tutorial hello
em++ -o $1.html $1.cpp -s WASM=1
