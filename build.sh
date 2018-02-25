#!/bin/bash

echo "---------- Go to Directory -----"
echo ""
cd ./build

echo "---------- Cmake ----------"
echo ""

cmake ..

echo "---------- Build ----------"
echo ""

cmake --build .

echo "---------- Move dll into Build/Modules folder ----------"
echo ""

cp ../Modules/* ./Modules

echo "---------- Finish ----------"
