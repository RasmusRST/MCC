# MATLAB matrix math to C++ Eigen Compiler #

This is a simple MATLAB to C++ Eigen compiler.
It handles the following operations:
Matrix addition
Matrix multiplication

### How to use ###
To compile the syntax analysis with flex/yacc, run cygwin:
```
./make.sh
```

To compile the compiler, run git bash (mingw) in the project root:
```
mkdir build
cd build
cmake .. -G "Visual Studio 14 Win64"
```

Run the mcc.sln in the build folder and set mcc as startup directory,
then everything should work.