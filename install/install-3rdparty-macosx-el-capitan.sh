#!/bin/bash
#
#  Copyright (C) 2016 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of WTSS.CXX.
#
#  WTSS.CXX is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  WTSS.CXX is distributed  "AS-IS" in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with WTSS.CXX . See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
#
#
#  Description: Install all required software for WTSS.CXX on Mac OS X El Capitan.
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#
#  Example:
#  $ WTSS_CXX_DIR="/Users/gribeiro/MyLibs" ./install-3rdparty-macosx-el-capitan.sh
#

echo "**********************************************************************"
echo "* WTSS.CXX_DIR 3rd-party Libraries Installer for Mac OS X El Capitan *"
echo "**********************************************************************"
echo ""
sleep 1s

#
# Valid parameter val or abort script
#
function valid()
{
  if [ $1 -ne 0 ]; then
    echo $2
    echo ""
    exit
  fi
}


#
# Check for tws-3rdparty-macosx-el-capitan.tar.gz
#
if [ ! -f ./wtss.cxx-3rdparty-macosx-el-capitan.tar.gz ]; then
  echo "Please, make sure to have wtss.cxx-3rdparty-macosx-el-capitan.tar.gz in the current directory!"
  echo ""
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
echo ""
sleep 1s

tar xzvf wtss.cxx-3rdparty-macosx-el-capitan.tar.gz
valid $? "Error: could not extract 3rd party libraries (wtss.cxx-3rdparty-macosx-el-capitan.tar.gz)"

echo "packages extracted!"
echo ""
sleep 1s


#
# Go to 3rd party libraries dir
#
cd wtss.cxx-3rdparty-macosx-el-capitan
valid $? "Error: could not enter 3rd-party libraries dir (wtss.cxx-3rdparty-macosx-el-capitan)"


#
# Check installation dir
#
if [ "$WTSS_CXX_DIR" == "" ]; then
  WTSS_CXX_DIR="/opt/wtss.cxx"
fi

if [ ! -d "$WTSS_CXX_DIR" ]; then
    mkdir $WTSS_CXX_DIR
fi

export PATH="$PATH:$WTSS_CXX_DIR/bin"
export LD_LIBRARY_PATH="$PATH:$WTSS_CXX_DIR/lib"

echo "installing 3rd-party libraries to '$WTSS_CXX_DIR' ..."
echo ""
sleep 1s


#
# RapidJSON
#
if [ ! -f "$WTSS_CXX_DIR/include/rapidjson/rapidjson.h" ]; then
  echo "installing RapidJSON..."
  echo ""
  sleep 1s

  tar xzvf rapidjson-0.11.tar.gz
  valid $? "Error: could not uncompress rapidjson-0.11.tar.gz!"

  cd rapidjson
  valid $? "Error: could not enter rapidjson!"

  mv include/rapidjson $WTSS_CXX_DIR/include/

  cd ..
fi


#
# cpp-netLib
#
if [ ! -f "$WTSS_CXX_DIR/include/boost/mime.hpp" ]; then
  echo "installing cpp-netlib..."
  echo ""
  sleep 1s

  tar xzvf cpp-netlib-0.11.2-final.tar.gz
  valid $? "Error: could not uncompress cpp-netlib-0.11.2-final.tar.gz!"

  cd cpp-netlib-0.11.2-final
  valid $? "Error: could not enter cpp-netlib-0.11.2-final!"

  mkdir build-cppnetlib

  cd build-cppnetlib
  valid $? "Error: could not enter build-cppnetlib!"

  #cmake ../ -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_PREFIX_PATH:PATH="$WTSS_CXX_DIR" -DCMAKE_INSTALL_PREFIX:PATH="$WTSS_CXX_DIR" -DCPP-NETLIB_BUILD_SHARED_LIBS:BOOL="ON" -DCPP-NETLIB_BUILD_TESTS:BOOL="OFF" -DCPP-NETLIB_BUILD_EXPERIMENTS:BOOL="OFF" -DCPP-NETLIB_BUILD_EXAMPLES:BOOL="OFF" -DCPP-NETLIB_ENABLE_HTTPS:BOOL="OFF" -DCMAKE_INSTALL_RPATH:PATH="$WTSS_CXX_DIR/lib" -DCMAKE_SKIP_BUILD_RPATH:BOOL="OFF" -DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL="OFF" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL="OFF"
  cmake ../ -DCMAKE_BUILD_TYPE:STRING="Release" -DCMAKE_PREFIX_PATH:PATH="$WTSS_CXX_DIR" -DCMAKE_INSTALL_PREFIX:PATH="$WTSS_CXX_DIR" -DCPP-NETLIB_BUILD_SHARED_LIBS:BOOL="ON" -DCPP-NETLIB_BUILD_TESTS:BOOL="OFF" -DCPP-NETLIB_BUILD_EXPERIMENTS:BOOL="OFF" -DCPP-NETLIB_BUILD_EXAMPLES:BOOL="OFF" -DCPP-NETLIB_ENABLE_HTTPS:BOOL="OFF"
  valid $? "Error: CMake failed for cpp-netlib!"

  make -j 4
  valid $? "Error: Build failed for cpp-netlib!"

  make install
  valid $? "Error: Installation failed for cpp-netlib!"

  cd ../..
fi


#
# Finished!
#
clear
echo "**********************************************************************"
echo "* WTSS.CXX_DIR 3rd-party Libraries Installer for Mac OS X El Capitan *"
echo "**********************************************************************"
echo ""
echo "finished successfully!"
