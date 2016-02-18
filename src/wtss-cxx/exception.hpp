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
  \file tws/exception.hpp

  \brief Base exception class exceptions in TerraLib Web Services.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __WTSS_CXX_EXCEPTION_HPP__
#define __WTSS_CXX_EXCEPTION_HPP__

// STL
#include <stdexcept>
#include <string>

// Boost
#include <boost/exception/all.hpp>

//! TerraLib Web Services namespace.
namespace wtss
{

  //! Base exception class for WTSS.CXX.
  struct exception: virtual std::exception, virtual boost::exception { };

  //! Type for handling missing parameter value in service request.
  struct missing_argument_error: virtual exception { };

  //! Type for handling invalid parameter value in service request.
  struct invalid_argument_error: virtual exception { };

  //! The base type for error report messages.
  typedef boost::error_info<struct tag_error_description, std::string> error_description;

}   // end namespace wtss

#endif // __WTSS_CXX_EXCEPTION_HPP__
