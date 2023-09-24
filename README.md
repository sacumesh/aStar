# README

## Authors

- Konara Mudiyansealge Sachiththa Umesh Bandaranayake
- Sanjana Raparthi


## Building and Running aStar

This is a guide on how to build and run aStar on different operating systems.

### Windows

To build aStar on Windows, you need to have the following dependencies installed:
- MinGW: A development environment for Windows. You can download it from [MinGW website](http://www.mingw.org/).

Once you have MinGW installed, you can follow these steps:

1. Open a command prompt.
2. Navigate to the aStar directory.
3. Run the following command: `mingw32-make`

This will compile the program and generate an executable named `aStar.exe`. You can then run the program by executing `aStar.exe`.

### Linux

To build aStar on Linux, you need to have the following dependencies installed:
- Make: A build automation tool. You can install it using the package manager of your Linux distribution.

Once you have Make installed, you can follow these steps:

1. Open a terminal.
2. Navigate to the aStar directory.
3. Run the following command: `make`

This will compile the program and generate an executable named `aStar`. You can then run the program by executing `./aStar` or `./aStar [departure city] [arrival city].`

### macOS

To build aStar on macOS, you need to have the following dependencies installed:
- Xcode Command Line Tools: A set of software development tools for macOS. You can install them by running `xcode-select --install` in a terminal.

Once you have Xcode Command Line Tools installed, you can follow these steps:

1. Open a terminal.
2. Navigate to the aStar directory.
3. Run the following command: `make`

This will compile the program and generate an executable named `aStar`. You can then run the program by executing `./aStar` or `./aStar [departure city] [arrival city].`.

## Generating Doxygen Documentation

To generate Doxygen documentation for aStar, you need to have the following dependencies installed:
- Doxygen: A documentation generator. You can download it from [Doxygen website](http://www.doxygen.nl/).

Once you have Doxygen installed, you can follow these steps:

1. Open a terminal.
2. Navigate to the aStar directory.
3. Run the following command: `make doc`

This will generate the Doxygen documentation in the `doc` directory.


To clean the object files, executable and generated documentation, you can use the following command:
`make clean`


## Distributing

To create a distribution archive, you can use the following command:
`make distribute`
