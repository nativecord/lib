@echo off
mkdir proj
cd proj
cmake ../ -DNATIVECORD_LIB_TEST=ON
cd ..
