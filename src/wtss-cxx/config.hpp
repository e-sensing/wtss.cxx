/*
  Copyright (C) 2016 National Institute For Space Research (INPE) - Brazil.

  This file is part of the WTSS.CXX.

  WTSS.CXX is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  WTSS.CXX is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with WTSS.CXX. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-cxx/config.hpp

  \brief Proxy file for the real wtss_cxx_config.hpp file.

  \author Matheus Cavassan Zaglia
 */

#ifndef __WTSS_CXX_CONFIG_HPP__
#define __WTSS_CXX_CONFIG_HPP__

// WTSS.CXX
#include "wtss_cxx_config.hpp"

#ifdef WIN32
  #ifdef WTSSCXXDLL
    #define WTSSCXXEXPORT __declspec(dllexport)
  #else
    #define WTSSCXXEXPORT __declspec(dllimport)
  #endif
#else
  #define WTSSCXXEXPORT
#endif

#endif  // __WTSS_CXX_CONFIG_HPP__
