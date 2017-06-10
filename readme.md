# MATLAB matrix math to C++ Eigen Conversion #

This is a simple MATLAB to C++ Eigen converter.
It handles the following operations:
Matrix addition
Matrix multiplication
Matrix indices
various MATLAB functions

### How to use ###
In respective command line (cmd / cygwin) run the following command:
```
./mcc testfile.m testfile.cpp
```

### How to compile ###
To compile the syntax analysis with flex/yacc, run cygwin:
```
./make.sh
```

To compile VS2015 solution, run git bash (mingw) in the project root:
```
mkdir build
cd build
cmake .. -G "Visual Studio 14 Win64"
```

Run the mcc.sln in the build folder and set mcc as startup directory,
then everything should work.

### Description of files ###
#### mcc.c ####
the main file.