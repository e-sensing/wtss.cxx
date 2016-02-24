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
  \file wtss-cxx/exception.hpp

  \brief Implementation of util class in WTSS.CXX.

  \author Matheus Cavassan Zaglia
 */

// wtss.cxx
#include "utils.hpp"
#include "exception.hpp"

//cpp-netlib
#include <boost/network/protocol/http/client.hpp>

// Boost
#include <boost/format.hpp>

rapidjson::Document
wtss_cxx::json_request(const std::string& server_uri)
{
  boost::network::http::client client;
  boost::network::http::client::request request(server_uri);
  boost::network::http::client::response response = client.get(request);

  std::string json = body(response);

  rapidjson::Document doc;

  doc.Parse<0>(json.c_str());

  if(doc.HasParseError())
  {
    boost::format err_msg("error parsing requested document '%1%': %2%.");

    throw parse_error() << error_description((err_msg % server_uri % doc.GetParseError()).str());
  }

  return doc;
}
