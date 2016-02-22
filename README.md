# C++ Client API for Web Time Series Service

**wtss.cxx** is a free and open source C++ client library for handling Web Time-Series Service (WTSS) in the client side. For more information on WTSS see  [its specification and documentation in the TWS site](https://github.com/e-sensing/tws). 

wtss.cxx is part of the [TerraLib family of geotechnologies](http://www.dpi.inpe.br/terralib5).

## Dependencies

The file named **[DEPENDENCIES](https://github.com/e-sensing/wtss.cxx/blob/master/DEPENDENCIES)** in the root of wtss.cxx source tree contains the official list of third-party libraries and tools that you must install before building wtss.cxx from source.

If you want to build yourself wtss.cxx then you need to install some third-party libraries. Below we show the list of third-party libraries dependencies and its versions. Please, take a look at the section "Building and Installing the Dependencies".

- **Boost (Mandatory):** wtss.cxx is built on top of Boost libraries. Make sure you have at least version 1.54.0 installed. If you prefer to install from source, download it from: http://www.boost.org.

- **RapidJSON (Mandatory):** wtss.cxx make use of RapidJSON library for decoding JSON documents. You can download it from: https://github.com/miloyip/rapidjson.

- **cpp-netlib (Mandatory):** it is a collection of open-source libraries for high level network programming. wtss.cxx is built on top of its support for HTTP client communication. Make sure to have version 0.11.2 installed. You can download it from: http://cpp-netlib.org.

**Note:**
- When using wtss.cxx on a machine with a installed version of SciDB you probably already have Boost libraries installed.
- If you have installed TerraLib you already have Boost and you don't need to install it again.

## Building and Installing the Dependencies

### Bash script for building all dependencies on Linux Ubuntu 14.04 LTS

We have prepared a special bash script for building and installing the dependencies on Linux Ubuntu 14.04 LTS. This script can be found in wtss.cxx source tree under *install* folder. Follow the steps below:

- Download the third-party libraries package used by the development team: [wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz](http://www.dpi.inpe.br/esensing-devel/wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz).

- Copy the script [install-3rdparty-linux-ubuntu-14.04.sh](https://github.com/e-sensing/wtss.cxx/tree/master/install/install-3rdparty-linux-ubuntu-14.04.sh) to the same folder you have downloaded the *wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz* package.

- Open the shell command line and call the script *install-3rdparty-linux-ubuntu-14.04.sh* setting the target to install all the stuffs from these third-party libraries and tools:
```
$ WTSS_CXX_DIR="/home/user/mylibs" ./install-3rdparty-linux-ubuntu-14.04.sh
```

**Note:** **Don't** choose as target location, a system folder such as **/usr** or **/usr/local**. Try some user specific folder.

### Bash script for building all dependencies on Mac OS X El Capitan

We have prepared a special bash script for building and installing the dependencies on Mac OS X El Capitan. This script can be found in TWS source tree under *install* folder. Follow the steps below:

- Download the third-party libraries package used by the development team: [wtss.cxx-3rdparty-macosx-el-capitan.tar.gz](http://www.dpi.inpe.br/esensing-devel/wtss.cxx-3rdparty-macosx-el-capitan.tar.gz).

- Copy the script [install-3rdparty-macosx-el-capitan.sh](https://github.com/e-sensing/wtss.cxx/tree/master/install/install-3rdparty-macosx-el-capitan.sh) to the same folder you have downloaded the *wtss.cxx-3rdparty-macosx-el-capitan.tar.gz* package.

- Open the shell command line.

- Make sure your CMake environment can be found in your PATH:
```
$ export PATH=$PATH:/Applications/CMake.app/Contents/bin
```

- In the shell command line, call the script *install-3rdparty-macosx-el-capitan.sh* setting the target to install all the stuffs from these third-party libraries and tools:
```
$ WTSS_CXX_DIR="/Users/user/mylibs" ./install-3rdparty-macosx-el-capitan.sh
```

**Note:** **Don't** choose as target location, a system folder such as **/usr** or **/usr/local**. Try some user specific folder.

## Cloning wtss.cxx Repository

- Open the shell command line.

- Make a new folder to host TWS source code:
```
$ mkdir -p /home/user/mydevel/wtss.cxx/codebase
```

- Change the current directory to that new folder:
```
$ cd /home/user/mydevel/wtss.cxx/codebase
```

- Make a local copy of TWS repository:
```
$ git clone https://github.com/e-sensing/wtss.cxx.git .
```

## Build Instructions

Make sure you have all the third-party library dependencies before trying to build wtss.cxx.

The `build/cmake` folder contains a CMake project for building wtss.cxx.

Until now its build has been tested on:
- Linux Ubuntu 14.04
- Mac OS X El Capitan

You should use at least CMake version 2.8.12 for building wtss.cxx. Older versions than this may not work properly.

Follow the build steps below according to your platform.

### Building on Linux with GNU G++

### Building on Mac OS X El Capitan

Consider to use the following flags:
- The wtss.cxx final library and header installation directory:
```
-DCMAKE_INSTALL_PREFIX:PATH="/Users/user_name/MyLibs"
```

- The places where CMake can search for 3rd-party libraries and auxiliary cmake scripts:
```
-DCMAKE_PREFIX_PATH:PATH="/Users/user_name/MyLibs;/Users/user_name/MyLibs/lib/cmake"
```

- If you want to build wtss.cxx on *Debug* mode:
```
-DCMAKE_BUILD_TYPE:STRING="Debug"
```
otherwise, just replace Debug in the above option for *Release*:

- If you prefer to inform the place where cppnetlib is installed, you can use the following option:
```
-Dcppnetlib_DIR:PATH="/Users/user_name/MyLibs/lib/cmake/cppnetlib"
```

### Building on Microsoft Windows with Visual C++

This is not supported yet! If you need to build on Windows, please, let us know!

## Quick Notes for Developers

## Reporting Bugs

Any problem should be reported to esensing-team@dpi.inpe.br.


For more information on wtss.cxx, please, visit its main web page at: http://www.dpi.inpe.br/esensing.
