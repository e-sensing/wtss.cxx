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
  with WTSS.CXX. See COPYING. If not, see
  <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-cxx/utils.hpp

  \brief Base util class  in WTSS.CXX.
  \author Matheus Cavassan Zaglia
 */

// STL
#include <string>

#ifndef __WTSS_CXX_UTILS_HPP__
#define __WTSS_CXX_UTILS_HPP__

namespace wtss
{
  namespace cxx
  {
    std::string request(const std::string& server_uri);
  } // end namespace cxx
}   // end namespace wtss

#endif  // __WTSS_CXX_UTILS_HPP__
