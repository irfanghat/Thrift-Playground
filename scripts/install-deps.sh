#!/bin/bash

sudo apt update

echo ""
echo "Installing dependencies..."
echo ""

sudo apt install libgtest-dev libgmock-dev thrift-compiler libthrift-dev -y

echo ""
echo "Verifying Thrift installation..."
echo ""

thrift --version
pkg-config --modversion thrift
