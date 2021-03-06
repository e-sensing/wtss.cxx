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
#  Description: Install all required software for WTSS.CXX on Linux Ubuntu 14.04 LTS.
#
#  Author: Matheus Cavassan Zaglia <mzaglia@dpi.inpe.br>
#
#
#  Example:
#  $ WTSS_CXX_DIR="/home/user/mylibs" ./install-3rdparty-linux-ubuntu-14.04.sh
#

echo "*************************************************************************"
echo "* WTSS.CXX_DIR 3rd-party Libraries Installer for Linux Ubuntu 14.04 LTS *"
echo "*************************************************************************"
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
if [ ! -f ./wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz ]; then
  echo "Please, make sure to have wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz in the current directory!"
  echo ""
  exit
fi


#
# Extract packages
#
echo "extracting packages..."
echo ""
sleep 1s

tar xzvf wtss.cxx-3rdparty-linux-ubuntu-14.04.tar.gz
valid $? "Error: could not extract 3rd party libraries (wtss.cxx-3rdparty-macosx-el-capitan.tar.gz)"

echo "packages extracted!"
echo ""
sleep 1s


#
# Go to 3rd party libraries dir
#
cd wtss.cxx-3rdparty-linux-ubuntu-14.04
valid $? "Error: could not enter 3rd-party libraries dir (wtss.cxx-3rdparty-linux-ubuntu-14.04)"


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

  cd rapidjson/include
  valid $? "Error: could not enter rapidjson!"

  mv rapidjson $WTSS_CXX_DIR/include/

  cd ..
fi


#
# CURL version 7.42.1
# Site: http://curl.haxx.se/libcurl/
#
if [ ! -f "$WTSS_CXX_DIR/lib/libcurl.so" ]; then
  echo "installing CURL..."
  sleep 1s

  tar xzvf curl-7.42.1.tar.gz
  valid $? "Error: could not uncompress curl-7.42.1.tar.gz!"

  cd curl-7.42.1
  valid $? "Error: could not enter curl-7.42.1!"

  ./configure --prefix=$WTSS_CXX_DIR
  valid $? "Error: could not configure CURL!"

  make -j 4
  valid $? "Error: could not make CURL!"

  make install
  valid $? "Error: Could not install CURL!"

  cd ..
fi



#
# Finished!
#
clear
echo "************************************************************************"
echo "* WTSS.CXX_DIR 3rd-party Libraries Installer for Linux Ubuntu 14.04 LTS*"
echo "************************************************************************"
echo ""
echo "finished successfully!"
