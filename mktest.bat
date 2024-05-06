@echo off
mkdir proj
cd proj
cmake ../ -DNATIVECORD_LIB_TEST=ON -DNATIVECORD_LIB_SHARED=ON -DCMAKE_BUILD_TYPE=Debug
cd ..
